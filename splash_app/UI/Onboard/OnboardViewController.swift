//
//  OnboardViewController.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class OnboardViewController: PageViewController {

    let viewControllers: [UIViewController] = [
        IntroViewController(),
        GitHubViewController(),
        ThemeViewController()
    ]

    override var isAnimatingViewChange: Bool {
        set {
            super.isAnimatingViewChange = newValue
            fixedView.pageControl.isEnabled = !newValue
        }
        get {
            return super.isAnimatingViewChange
        }
    }

    var canChangePage: Bool {
        return !isAnimatingViewChange
    }

    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {return .portrait}
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return ThemeManager.shared.theme.statusBarStyle
    }

    var currentViewController: UIViewController?
    var currentPageIndex: Int? {
        guard let currentViewController = currentViewController else {return nil}
        return viewControllers.firstIndex(of: currentViewController)
    }

    let fixedView = FixedView()

    var observers = [Any]()

    override func loadView() {
        super.loadView()

        fixedView.setupForAutoLayout(in: view)
        fixedView.pinToSuperview()

        totalPageCount = viewControllers.count
        self.pageIndicatorDelegate = fixedView.pageControl

        let firstViewController = viewControllers.first!
        currentViewController = firstViewController
        present(firstViewController, at: 0, animated: false)
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        viewControllers
            .compactMap {$0 as? GitHubViewController}
            .forEach {_ = $0.view} // load view to load github page
        observers.append(  // swiftlint:disable:next discarded_notification_center_observer
            NotificationCenter.default.addObserver(forName: .themeChanged,
                                                   object: nil,
                                                   queue: nil,
                                                   using: { _ in
                                                    self.setupAppearance()
            }))
        setupAppearance()
    }

    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()

        let bottomViewHeight = fixedView.bottomView.frame.height
        let safeAreaInsets = UIEdgeInsets(top: 0, left: 0, bottom: bottomViewHeight, right: 0)
        viewControllers.forEach {$0.additionalSafeAreaInsets = safeAreaInsets}
    }

    deinit {
        observers.forEach(NotificationCenter.default.removeObserver)
    }

    func setupAppearance() {
        let theme = ThemeManager.shared.theme
        view.backgroundColor = theme.backgroundColor
        setNeedsStatusBarAppearanceUpdate()
    }

    // MARK: - Page Control

    @objc
    func advance() {
        guard let currentPageIndex = currentPageIndex else {return}

        setPage(at: currentPageIndex + 1)
    }

    @objc
    func pageControlChanged(sender: UIPageControl) {
        setPage(at: sender.currentPage)
    }

    func setPage(at index: Int) {
        guard canChangePage else {return}
        let viewControllersRange = viewControllers.startIndex..<viewControllers.endIndex
        guard viewControllersRange.contains(index) else {return}

        self.currentViewController = viewControllers[index]
        present(self.currentViewController!, at: index, animated: true)

        let isLastPage = index == viewControllersRange.upperBound - 1
        fixedView.buttonState = isLastPage ? .finish : .continue
    }
}
