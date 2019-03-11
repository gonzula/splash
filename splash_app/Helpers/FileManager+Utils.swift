//
//  FileManager+Utils.swift
//  splash
//
//  Created by Gonzo Fialho on 10/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import Foundation

extension FileManager {
    static var documentsDirectory: URL {
        let paths = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)
        let documentsDirectory = paths[0]
        return documentsDirectory
    }
}
