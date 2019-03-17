//
//  OnboardViewController+Github.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit
import SafariServices

extension OnboardViewController {
    class GitHubViewController: InsidePageViewController {
        let githubView = UIView()
        let safariVC = SFSafariViewController(url: URL(string: "https://github.com/gonzula/splash")!)

        override func loadView() {
            super.loadView()

            title = "Splash is completely open source"
            caption = """
            So, if you find a bug, have a feature request, want to know what new features are being developed or want \
            to contribute to the project you can visit the project page on GitHub.
            """

            githubView.setupForAutoLayout(in: contentView)
            githubView.pinToSuperview()

            addChild(safariVC)
            githubView.addSubview(safariVC.view)
            safariVC.didMove(toParent: self)
        }

        override func viewDidAppear(_ animated: Bool) {
            super.viewDidAppear(animated)

            safariVC.view.frame = githubView.bounds
            removeView(ofClass: UINavigationBar.self, from: githubView)
            removeView(ofClass: UIToolbar.self, from: githubView)
        }

        @discardableResult
        func removeView<T: UIView>(ofClass `class`: T.Type, from view: UIView) -> Bool {
            if let view = view as? T {
                print(view)
                view.removeFromSuperview()
                return true
            }

            for subview in view.subviews {
                let found = removeView(ofClass: T.self, from: subview)
                if found {
                    return true
                }
            }
            return false
        }
    }
}
