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

        let insideView = InsideView()

        override var title: String? {
            set {
                super.title = newValue
                insideView.titleLabel.text = title
            }
            get {
                return super.title
            }
        }

        var caption: String? {
            set {
                insideView.captionLabel.text = newValue
            }
            get {
                return insideView.captionLabel.text
            }
        }

        var contentView: UIView {return insideView.contentView}

        var observers = [Any]()

        override func loadView() {
            view = insideView
        }

        override func viewDidLoad() {
            super.viewDidLoad()

            observers.append(  // swiftlint:disable:next discarded_notification_center_observer
                NotificationCenter.default.addObserver(forName: .themeChanged,
                                                       object: nil,
                                                       queue: nil,
                                                       using: { [weak self] _ in
                                                        self?.setupAppearance()
                })
            )
            setupAppearance()
        }

        deinit {
            observers.forEach(NotificationCenter.default.removeObserver)
        }

        func setupAppearance() {
            let theme = ThemeManager.shared.theme
            insideView.titleLabel.textColor = theme.textColor
            insideView.captionLabel.textColor = theme.textColor
        }
    }
}

extension OnboardViewController {
    class InsideView: UIView {
        let titleLabel = UILabel()
        let captionLabel = UILabel()

        let contentView = UIView()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

        private func setup() {
            setupTitleLabel()
            setupCaptionLabel()
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

            titleLabel.font = .preferredFont(forTextStyle: .title1)
            titleLabel.adjustsFontForContentSizeCategory = true
            titleLabel.numberOfLines = 0
        }

        fileprivate func setupCaptionLabel() {
            captionLabel.setupForAutoLayout(in: self)

            captionLabel.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: 20).activate()
            captionLabel.leftAnchor.constraint(equalTo: safeAreaLayoutGuide.leftAnchor,
                                                   constant: 30).activate()
            captionLabel.rightAnchor.constraint(equalTo: safeAreaLayoutGuide.rightAnchor,
                                                    constant: -30)
                .setPriority(999)
                .activate()

            captionLabel.font = .preferredFont(forTextStyle: .body)
            captionLabel.adjustsFontSizeToFitWidth = true
            captionLabel.numberOfLines = 0
        }

        fileprivate func setupContentView() {
            contentView.setupForAutoLayout(in: self)

            contentView.topAnchor.constraint(equalTo: captionLabel.bottomAnchor,
                                             constant: 20).activate()
            contentView.leftAnchor.constraint(equalTo: leftAnchor).activate()
            contentView.rightAnchor.constraint(equalTo: rightAnchor).activate()
            contentView.bottomAnchor.constraint(equalTo: bottomAnchor).setPriority(999).activate()
        }
    }
}
