//
//  UserDefaults+Constants.swift
//  splash
//
//  Created by Gonzo Fialho on 12/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import Foundation

extension UserDefaults.Domain {
    static let investor = UserDefaults.Domain(name: "splash_app")
}

extension UserDefaults.Key {
    static let lastUsedTheme = UserDefaults.Key(domain: .investor, keyName: "current theme")
}

extension UserDefaults {
    var lastUsedTheme: ThemeManager.Theme {
        get {
            guard let themeName = string(forKey: .lastUsedTheme),
                let theme = ThemeManager.Theme(rawValue: themeName) else {
                    return .light  // default theme
            }
            return theme
        }
        set {
            set(newValue.rawValue, forKey: .lastUsedTheme)
        }
    }
}
