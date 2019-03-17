//
//  ThemeControl.swift
//  splash
//
//  Created by Gonzo Fialho on 17/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class ThemeControl: UISegmentedControl {

    var observers = [Any]()

    init() {
        let themes = ThemeManager.Theme.allCases
        super.init(items: themes.map {$0.humanName})
        setValue()

        addTarget(self,
                  action: #selector(changeTheme),
                  for: .valueChanged)

        observers.append( // swiftlint:disable:next discarded_notification_center_observer
            NotificationCenter.default.addObserver(forName: .themeChanged,
                                                   object: nil,
                                                   queue: nil,
                                                   using: { [weak self] _ in
                                                    self?.setValue()
            }))
    }

    override init(frame: CGRect) {
        super.init(frame: frame)
    }

    deinit {
        observers.forEach(NotificationCenter.default.removeObserver)
    }

    required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

    func setValue() {
        let themes = ThemeManager.Theme.allCases
        let selectedTheme = UserDefaults.standard.lastUsedTheme
        let index = themes.firstIndex(of: selectedTheme)
        guard self.selectedSegmentIndex != index else {return}
        selectedSegmentIndex = index ?? -1
    }

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
