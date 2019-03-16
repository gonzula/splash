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
            (view as? IntroView)?.continueAction = { [unowned self] in
                (self.parent as? OnboardViewController)?.advance()
            }
        }
    }
}

extension OnboardViewController {
    class IntroView: UIView {
        let titleLabel = UILabel()
        let descriptionLabel = UILabel()

        let button = UIButton(type: .system)

        var continueAction: (() -> Void)?

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
            setupButton()
        }

        fileprivate func setupTitleLabel() {
            titleLabel.setupForAutoLayout(in: self)

            titleLabel.topAnchor.constraint(equalTo: safeAreaLayoutGuide.topAnchor, constant: 44).isActive = true
            titleLabel.leftAnchor.constraint(equalTo: safeAreaLayoutGuide.leftAnchor, constant: 20).isActive = true
            let constraint = titleLabel.rightAnchor.constraint(lessThanOrEqualTo: safeAreaLayoutGuide.rightAnchor,
                                              constant: -20)
            constraint.priority = UILayoutPriority(rawValue: 999)
            constraint.isActive = true

            titleLabel.text = "Welcome to Splash!"
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
            Splash is a programming language that helps you to create your (complex) shortcuts.
            """
            descriptionLabel.font = .preferredFont(forTextStyle: .body)
            descriptionLabel.adjustsFontSizeToFitWidth = true
            descriptionLabel.numberOfLines = 0
        }

        fileprivate func setupButton() {
            button.setupForAutoLayout(in: self)

            button.centerXAnchor.constraint(equalTo: safeAreaLayoutGuide.centerXAnchor).isActive = true
            let constraint = button.leftAnchor.constraint(greaterThanOrEqualTo: safeAreaLayoutGuide.leftAnchor,
                                                          constant: 20)
            constraint.priority = UILayoutPriority(rawValue: 999)
            constraint.isActive = true
            button.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor, constant: -20).isActive = true

            button.setTitle("Continue", for: .normal)

            button.addTarget(self, action: #selector(continueTouched), for: .touchUpInside)
        }

        // MARK: - User Interaction

        @objc func continueTouched() {
            continueAction?()
        }
    }
}
