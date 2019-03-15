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

    let fixedView = FixedContent()
    var pageControl: UIPageControl {return fixedView.pageControl}

    override func loadView() {
        super.loadView()

        fixedView.setupForAutoLayout(in: view)
        fixedView.pinToSuperview()

        for viewController in viewControllers {
            viewController.additionalSafeAreaInsets = UIEdgeInsets(top: 0, left: 0, bottom: 44, right: 0)
        }

        totalPageCount = viewControllers.count
        self.pageIndicatorDelegate = pageControl

        let firstViewController = viewControllers.first!
        present(firstViewController, at: 0, animated: false)
    }
}
