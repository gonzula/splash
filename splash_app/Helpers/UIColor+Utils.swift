//
//  UIColor+Utils.swift
//  splash
//
//  Created by Gonzo Fialho on 04/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension UIColor {
    // swiftlint:disable colon
    convenience init(hex: Int, alpha: CGFloat = 1.0) {
        self.init(red:   CGFloat((hex & 0xff0000 ) >> 0x10 ) / 0xff,
                  green: CGFloat((hex & 0x00ff00 ) >> 0x08 ) / 0xff,
                  blue:  CGFloat((hex & 0x0000ff ) >> 0x00 ) / 0xff,
                  alpha: alpha)
    }
    // swiftlint:enable colon
    var components: [CGFloat] {
        var fRed: CGFloat = 0
        var fGreen: CGFloat = 0
        var fBlue: CGFloat = 0
        var fAlpha: CGFloat = 0
        self.getRed(&fRed,
                    green: &fGreen,
                    blue: &fBlue,
                    alpha: &fAlpha)
        return [fRed, fGreen, fBlue, fAlpha]
    }
}
