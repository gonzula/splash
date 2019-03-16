//
//  NSLayoutConstraint+Utils.swift
//  splash
//
//  Created by Gonzo Fialho on 15/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension NSLayoutConstraint {
    @discardableResult
    func setPriority(_ value: Float) -> NSLayoutConstraint {
        self.priority = UILayoutPriority(rawValue: value)
        return self
    }

    @discardableResult
    func activate() -> NSLayoutConstraint {
        self.isActive = true
        return self
    }
}
