//
//  String+Utils.swift
//  splash
//
//  Created by Gonzo Fialho on 16/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import Foundation

public extension StringProtocol {
    func capitalizingFirstLetter() -> String {
        return prefix(1).uppercased() + dropFirst()
    }
}

public extension String {
    mutating func capitalizeFirstLetter() {
        self = self.capitalizingFirstLetter()
    }
}
