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
        IntroViewController()
    ]

    let pageControl = UIPageControl()

    override func loadView() {
        super.loadView()

        pageControl.setupForAutoLayout(in: view)
        pageControl.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor,
                                            constant: -8).isActive = true
        pageControl.centerXAnchor.constraint(equalTo: view.safeAreaLayoutGuide.centerXAnchor).isActive = true

        pageControl.pageIndicatorTintColor = .lightGray
        pageControl.currentPageIndicatorTintColor = .black

        for viewController in viewControllers {
            viewController.additionalSafeAreaInsets = UIEdgeInsets(top: 0, left: 0, bottom: 20, right: 0)
        }

        totalPageCount = viewControllers.count
        self.pageIndicatorDelegate = pageControl

        let firstViewController = viewControllers.first!
        present(firstViewController, at: 0, animated: false)
    }
}
