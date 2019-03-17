//
//  OnboardViewController+Github.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit
import WebKit

extension OnboardViewController {
    class GitHubViewController: InsidePageViewController {
        override func loadView() {
            super.loadView()

            title = "Splash is completely open source"
            caption = """
            So, if you find a bug, have a feature request, want to know what new features are being developed or want \
            to contribute to the project you can visit the project page on GitHub.
            """

            let githubView = GitHubView()
            githubView.setupForAutoLayout(in: contentView)
            githubView.pinToSuperview()
        }
    }
}

extension OnboardViewController {
    class GitHubView: UIView {
        let webView = WKWebView()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

        private func setup() {
            setupWebView()
        }

        fileprivate func setupWebView() {
            webView.setupForAutoLayout(in: self)
            webView.isOpaque = false
            webView.backgroundColor = UIColor.clear
            webView.scrollView.backgroundColor = UIColor.clear

            let url = URL(string: "https://github.com/gonzula/splash")!
            webView.load(URLRequest(url: url))

            webView.pinToSuperview()
        }
    }
}
