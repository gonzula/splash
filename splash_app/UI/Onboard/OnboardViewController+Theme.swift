//
//  OnboardViewController+Theme.swift
//  splash
//
//  Created by Gonzo Fialho on 16/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class ThemeViewController: UIViewController {
        override func loadView() {
            view = ThemeView()
        }

        // MARK: - User Interaction

        @objc
        func changeTheme(sender: UISegmentedControl) {

        }
    }
}

extension OnboardViewController {
    class ThemeView: UIView {

        let themes = ThemeManager.Theme.allCases
        let segmentedControl = UISegmentedControl()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            setupSegmentedControl()
        }

        private func setupSegmentedControl() {
            segmentedControl.setupForAutoLayout(in: self)

            themes
                .enumerated()
                .forEach {segmentedControl.insertSegment(withTitle: $0.1.humanName,
                                                         at: $0.0,
                                                         animated: false)}

            let selectedTheme = UserDefaults.standard.lastUsedTheme
            let index = themes.firstIndex(of: selectedTheme)
            segmentedControl.selectedSegmentIndex = index ?? -1

            segmentedControl.addTarget(nil,
                                       action: #selector(ThemeViewController.changeTheme(sender:)),
                                       for: .valueChanged)

            segmentedControl.centerXAnchor.constraint(equalTo: centerXAnchor).activate()
            segmentedControl.topAnchor.constraint(equalTo: safeAreaLayoutGuide.topAnchor,
                                                  constant: 20).activate()
        }
    }
}
