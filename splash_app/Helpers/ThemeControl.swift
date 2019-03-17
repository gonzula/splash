//
//  ThemeControl.swift
//  splash
//
//  Created by Gonzo Fialho on 17/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class ThemeControl: UISegmentedControl {
    init() {
        let themes = ThemeManager.Theme.allCases
        super.init(items: themes.map {$0.humanName})
        let selectedTheme = UserDefaults.standard.lastUsedTheme
        let index = themes.firstIndex(of: selectedTheme)
        selectedSegmentIndex = index ?? -1

        addTarget(self,
                  action: #selector(changeTheme),
                  for: .valueChanged)
    }

    override init(frame: CGRect) {
        super.init(frame: frame)
    }

    required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

    // MARK: - User Interaction

    @objc
    func changeTheme() {
        let themes = ThemeManager.Theme.allCases
        let themesRange = themes.startIndex..<themes.endIndex
        let index = selectedSegmentIndex
        guard themesRange.contains(index) else {return}
        let theme = themes[index]
        ThemeManager.shared.theme = theme
    }
}
