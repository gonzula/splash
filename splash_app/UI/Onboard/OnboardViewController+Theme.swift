//
//  OnboardViewController+Theme.swift
//  splash
//
//  Created by Gonzo Fialho on 16/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class ThemeViewController: InsidePageViewController {
        override func loadView() {
            super.loadView()

            title = "One more thing…"
            caption = "Please choose the appearence of the your app (you can change it later)."

            let themeView = ThemeView()
            themeView.setupForAutoLayout(in: contentView)
            themeView.pinToSuperview()
        }

        override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
            view.endEditing(true)
            super.touchesBegan(touches, with: event)
        }

        // MARK: - User Interaction

        @objc
        func changeTheme(sender: UISegmentedControl) {
            view.endEditing(true)

            let themes = ThemeManager.Theme.allCases
            let themesRange = themes.startIndex..<themes.endIndex
            let index = sender.selectedSegmentIndex
            guard themesRange.contains(index) else {return}
            let theme = themes[index]
            ThemeManager.shared.theme = theme
        }
    }
}

extension OnboardViewController {
    class ThemeView: UIView {

        let themes = ThemeManager.Theme.allCases
        let segmentedControl = UISegmentedControl()

        let lightEditor = EditorView()
        let darkEditor = EditorView()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            setupSegmentedControl()
            setupLightEditor()
            setupDarkEditor()
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

        private func setupLightEditor() {
            lightEditor.setupForAutoLayout(in: self)
            lightEditor.forcedTheme = .light

            lightEditor.topAnchor.constraint(equalTo: segmentedControl.bottomAnchor, constant: 20).activate()
            lightEditor.leftAnchor.constraint(equalTo: leftAnchor).activate()
            lightEditor.rightAnchor.constraint(equalTo: rightAnchor).activate()
            lightEditor.heightAnchor.constraint(equalToConstant: 44).activate()

            lightEditor.text = "ShowResult(\"This is the light theme\")"
            lightEditor.colorizeText()
        }

        private func setupDarkEditor() {
            darkEditor.setupForAutoLayout(in: self)
            darkEditor.forcedTheme = .dark

            darkEditor.topAnchor.constraint(equalTo: lightEditor.bottomAnchor, constant: 20).activate()
            darkEditor.leftAnchor.constraint(equalTo: leftAnchor).activate()
            darkEditor.rightAnchor.constraint(equalTo: rightAnchor).activate()
            darkEditor.heightAnchor.constraint(equalToConstant: 44).activate()

            darkEditor.text = "ShowResult(\"This is the dark theme\")"
            darkEditor.colorizeText()
        }
    }
}
