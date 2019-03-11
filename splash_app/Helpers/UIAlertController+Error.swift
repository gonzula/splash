//
//  UIAlertController+Error.swift
//  splash
//
//  Created by Gonzo Fialho on 03/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension UIAlertController {
    convenience init(error: Error) {
        self.init(title: "Error",
                  message: error.localizedDescription,
                  preferredStyle: .alert)

        self.addAction(UIAlertAction(title: "Ok",
                                     style: .default,
                                     handler: nil))
    }
}
