//
//  PageViewController.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

public protocol PageIndicator: class {
    func set(currentPage: Int, animated: Bool)
    var numberOfPages: Int {get set}
}

open class PageViewController: UIViewController {

    var isAnimatingViewChange: Bool = false
    open var updatesPageIndicatorAutomatically = true
    open weak var pageIndicatorDelegate: PageIndicator?

    private var currentViewController: UIViewController?
    private var currentIndex: Int?

    let contentView = UIView()

    open var totalPageCount: Int = 0 {
        didSet {
            guard let currentIndex = currentIndex, updatesPageIndicatorAutomatically else {return}
            setPageControl(page: currentIndex, animated: view.window != nil)
        }
    }

    override open var shouldAutomaticallyForwardAppearanceMethods: Bool {
        return false
    }

    open override func loadView() {
        view = UIView()
        view.backgroundColor = .white

        contentView.setupForAutoLayout(in: view)

        contentView.backgroundColor = .clear

        contentView.pinToSuperview()
    }

    fileprivate func setPageControl(page: Int, animated: Bool) {
        pageIndicatorDelegate?.set(currentPage: page, animated: animated)
        pageIndicatorDelegate?.numberOfPages = totalPageCount
    }

    open override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()

        guard let currentViewController = currentViewController, let currentIndex = currentIndex else {return}

        currentViewController.view.frame = CGRect(x: offset(for: currentIndex),
                                                  y: 0,
                                                  width: view.bounds.width,
                                                  height: view.bounds.height)
    }

    private func add(_ viewController: UIViewController, at index: Int) {
        addChild(viewController)
        viewController.view.frame = CGRect(x: offset(for: index),
                                           y: 0,
                                           width: view.bounds.width,
                                           height: view.bounds.height)
        contentView.addSubview(viewController.view)
        viewController.didMove(toParent: self)
    }

    fileprivate func remove(_ viewController: UIViewController?) {
        guard let viewController = viewController else {return}
        guard !viewController.isModal else {return}

        viewController.willMove(toParent: nil)
        viewController.view.removeFromSuperview()
        viewController.removeFromParent()
    }

    private func set(_ viewController: UIViewController,
                     at index: Int,
                     animated: Bool,
                     completion: (() -> Void)? = nil) {
        add(viewController, at: index)
        var viewControllerToDismiss: UIViewController?
        if !(currentViewController?.isModal ?? true) {
            viewControllerToDismiss = currentViewController
        }

        viewController.beginAppearanceTransition(true, animated: animated)
        viewControllerToDismiss?.beginAppearanceTransition(false, animated: animated)

        if animated {
            isAnimatingViewChange = true
            UIView.animate(withDuration: 0.3, animations: {
                self.contentView.bounds.origin.x = self.offset(for: index)
            }, completion: {(_) in
                self.isAnimatingViewChange = false
                viewController.endAppearanceTransition()
                viewControllerToDismiss?.endAppearanceTransition()
                completion?()
            })
        } else {
            contentView.bounds.origin.x = offset(for: index)
            viewController.endAppearanceTransition()
            viewControllerToDismiss?.endAppearanceTransition()
            completion?()
        }
    }

    open func present(_ viewController: UIViewController, at index: Int, animated: Bool) {
        if updatesPageIndicatorAutomatically {
            setPageControl(page: index, animated: animated)
        }
        view.endEditing(true)
        let viewControllerToRemove = currentViewController
        if viewControllerToRemove?.isModal ?? false {
            viewControllerToRemove!.dismiss(animated: animated, completion: nil)
            self.set(viewController, at: index, animated: false)
            self.currentViewController = viewController
            self.currentIndex = index
        } else {
            self.set(viewController, at: index, animated: animated) {
                self.remove(viewControllerToRemove)
                self.currentViewController = viewController
                self.currentIndex = index
            }
        }
    }

    open func presentModally(_ viewController: UIViewController) {
        view.endEditing(true)

        let viewControllerToRemove = currentViewController
        if viewControllerToRemove?.isModal ?? false {
            viewControllerToRemove!.dismiss(animated: true) {
                self.present(viewController, animated: true) {
                    self.currentViewController = viewController
                    self.currentIndex = nil
                }
            }
        } else {
            present(viewController, animated: true) {
                self.remove(viewControllerToRemove)
                self.currentViewController = viewController
                self.currentIndex = nil
            }
        }
    }

    private func offset(`for` index: Int) -> CGFloat {
        return CGFloat(index) * view.frame.width
    }

    // MARK: - User Interaction
}

fileprivate extension UIViewController {
    var isModal: Bool {
        return self.presentingViewController?.presentedViewController == self
    }
}

extension UIPageControl: PageIndicator {
    public func set(currentPage: Int, animated: Bool) {
        self.currentPage = currentPage
    }
}
