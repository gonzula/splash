//
//  OnboardViewController+Intro.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class IntroViewController: UIViewController {
        override func loadView() {
            view = IntroView()
        }
    }
}

extension OnboardViewController {
    class IntroView: UIView {
        let titleLabel = UILabel()
        let descriptionLabel = UILabel()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            backgroundColor = .clear

            setupTitleLabel()
            setupDescriptionLabel()
        }

        fileprivate func setupTitleLabel() {
            titleLabel.setupForAutoLayout(in: self)

            titleLabel.topAnchor.constraint(equalTo: safeAreaLayoutGuide.topAnchor, constant: 20).activate()
            titleLabel.leftAnchor.constraint(equalTo: safeAreaLayoutGuide.leftAnchor, constant: 20).activate()
            titleLabel.rightAnchor.constraint(lessThanOrEqualTo: safeAreaLayoutGuide.rightAnchor,
                                              constant: -20)
                .setPriority(999)
                .activate()

            titleLabel.text = "Welcome to Splash!"
            titleLabel.font = .preferredFont(forTextStyle: .title1)
            titleLabel.adjustsFontForContentSizeCategory = true
            titleLabel.numberOfLines = 0
        }

        fileprivate func setupDescriptionLabel() {
            descriptionLabel.setupForAutoLayout(in: self)

            descriptionLabel.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: 44).activate()
            descriptionLabel.leftAnchor.constraint(equalTo: safeAreaLayoutGuide.leftAnchor,
                                                   constant: 30).activate()
            descriptionLabel.rightAnchor.constraint(equalTo: safeAreaLayoutGuide.rightAnchor,
                                                    constant: -30)
                .setPriority(999)
                .activate()

            descriptionLabel.text = """
            Splash is a programming language that helps you to create your (complex) shortcuts.
            """
            descriptionLabel.font = .preferredFont(forTextStyle: .body)
            descriptionLabel.adjustsFontSizeToFitWidth = true
            descriptionLabel.numberOfLines = 0
        }
    }
}
