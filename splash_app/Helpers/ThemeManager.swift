//
//  ThemeManager.swift
//  splash
//
//  Created by Gonzo Fialho on 12/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class ThemeManager {

    static let shared = ThemeManager()

    var theme: Theme = UserDefaults.standard.lastUsedTheme {
        didSet {
            guard theme != oldValue else {return}
            UserDefaults.standard.lastUsedTheme = theme
            NotificationCenter.default.post(name: .themeChanged, object: theme)
        }
    }

    var backgroundColor: UIColor {
        switch theme {
        case .light: return .white
        case .dark: return UIColor(hex: 0x1F1F24)
        }
    }

    var keyboardAppearance: UIKeyboardAppearance {
        switch theme {
        case .light: return .light
        case .dark: return .dark
        }
    }

    var navigationBarStyle: UIBarStyle {
        switch theme {
        case .light: return .default
        case .dark: return .black
        }
    }
}

extension ThemeManager {
    enum Theme: String, CaseIterable {
        case light
        case dark

        var humanName: String {
            return rawValue.capitalizingFirstLetter()
        }
    }
}

extension ThemeManager.Theme {
    var other: ThemeManager.Theme {
        switch self {
        case .light: return .dark
        case .dark: return .light
        }
    }
}
