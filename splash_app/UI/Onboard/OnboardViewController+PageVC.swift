//
//  OnboardViewController+PageVC.swift
//  splash
//
//  Created by Gonzo Fialho on 16/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class InsidePageViewController: UIViewController {

        var contentView: UIView {return (view as! InsideView).contentView}

        override func loadView() {
            view = InsideView()
        }
    }
}

extension OnboardViewController {
    class InsideView: UIView {
        let titleLabel = UILabel()
        let descriptionLabel = UILabel()

        let contentView = UIView()

        override init(frame: CGRect) {
            super.init(frame: frame)
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            setupTitleLabel()
            setupDescriptionLabel()
            setupContentView()
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

        fileprivate func setupContentView() {
            contentView.setupForAutoLayout(in: self)

            contentView.topAnchor.constraint(equalTo: descriptionLabel.bottomAnchor,
                                             constant: 20).activate()
            contentView.leftAnchor.constraint(equalTo: leftAnchor).activate()
            contentView.rightAnchor.constraint(equalTo: rightAnchor).activate()
            contentView.bottomAnchor.constraint(equalTo: bottomAnchor).activate()
        }
    }
}
