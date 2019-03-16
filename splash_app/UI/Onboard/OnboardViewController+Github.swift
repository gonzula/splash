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
    class GitHubViewController: UIViewController {
        override func loadView() {
            view = GitHubView()
        }
    }
}

extension OnboardViewController {
    class GitHubView: UIView {

        let titleLabel = UILabel()
        let descriptionLabel = UILabel()
        let webView = WKWebView()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            setupTitleLabel()
            setupDescriptionLabel()
            setupWebView()
        }

        fileprivate func setupTitleLabel() {
            titleLabel.setupForAutoLayout(in: self)

            titleLabel.topAnchor.constraint(equalTo: safeAreaLayoutGuide.topAnchor, constant: 44).isActive = true
            titleLabel.leftAnchor.constraint(equalTo: safeAreaLayoutGuide.leftAnchor, constant: 20).isActive = true
            titleLabel.rightAnchor.constraint(lessThanOrEqualTo: safeAreaLayoutGuide.rightAnchor,
                                              constant: -20).isActive = true

            titleLabel.text = "Splash is completely open source"
            titleLabel.font = .preferredFont(forTextStyle: .title1)
            titleLabel.adjustsFontForContentSizeCategory = true
            titleLabel.numberOfLines = 0
        }

        fileprivate func setupDescriptionLabel() {
            descriptionLabel.setupForAutoLayout(in: self)

            descriptionLabel.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: 44).isActive = true
            descriptionLabel.leftAnchor.constraint(equalTo: safeAreaLayoutGuide.leftAnchor,
                                                   constant: 30).isActive = true
            let constraint = descriptionLabel.rightAnchor.constraint(equalTo: safeAreaLayoutGuide.rightAnchor,
                                                                     constant: -30)
            constraint.priority = UILayoutPriority(rawValue: 999)
            constraint.isActive = true

            descriptionLabel.text = """
            So, if you find a bug, have a feature request, want to know how the development is going or want to contribute to the code you can visit the project page on GitHub
            """
            descriptionLabel.font = .preferredFont(forTextStyle: .body)
            descriptionLabel.adjustsFontSizeToFitWidth = true
            descriptionLabel.numberOfLines = 0
        }

        fileprivate func setupWebView() {
            webView.setupForAutoLayout(in: self)

            let url = URL(string: "https://github.com/gonzula/splash")!
            webView.load(URLRequest(url: url))

            webView.topAnchor.constraint(equalTo: descriptionLabel.bottomAnchor, constant: 20).isActive = true
            webView.leftAnchor.constraint(equalTo: leftAnchor).isActive = true
            webView.rightAnchor.constraint(equalTo: rightAnchor).isActive = true
            webView.bottomAnchor.constraint(equalTo: bottomAnchor).isActive = true
        }
    }
}
