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
}

extension ThemeManager {
    enum Theme: String, CaseIterable {
        case light
        case dark

        var humanName: String {
            return rawValue.capitalizingFirstLetter()
        }

        var backgroundColor: UIColor {
            switch self {
            case .light: return .white
            case .dark: return UIColor(hex: 0x1F1F24)
            }
        }

        var other: ThemeManager.Theme {
            switch self {
            case .light: return .dark
            case .dark: return .light
            }
        }

        var keyboardAppearance: UIKeyboardAppearance {
            switch self {
            case .light: return .light
            case .dark: return .dark
            }
        }

        var navigationBarStyle: UIBarStyle {
            switch self {
            case .light: return .default
            case .dark: return .black
            }
        }
    }
}
