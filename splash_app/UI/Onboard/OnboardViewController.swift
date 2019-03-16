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
        GitHubViewController()
    ]

    var currentViewController: UIViewController?

    let fixedView = FixedContent()
    var pageControl: UIPageControl {return fixedView.pageControl}

    override func loadView() {
        super.loadView()

        fixedView.translatesAutoresizingMaskIntoConstraints = false
        view.insertSubview(fixedView, belowSubview: contentView)
        fixedView.pinToSuperview()

        for viewController in viewControllers {
            viewController.additionalSafeAreaInsets = UIEdgeInsets(top: 0, left: 0, bottom: 44, right: 0)
        }

        totalPageCount = viewControllers.count
        self.pageIndicatorDelegate = pageControl

        let firstViewController = viewControllers.first!
        currentViewController = firstViewController
        present(firstViewController, at: 0, animated: false)
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        viewControllers
            .compactMap {$0 as? GitHubViewController}
            .forEach {_ = $0.view} // load view to load github page
    }

    func advance() {
        guard let currentViewController = currentViewController else {return}
        guard let index = viewControllers.firstIndex(of: currentViewController),
            index < viewControllers.count - 1 else {return}

        self.currentViewController = viewControllers[index + 1]
        present(self.currentViewController!, at: index + 1, animated: true)
    }
}
