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

    static func createExamplesDirectory() {
        let documentsPath = FileManager.documentsDirectory.path
        let examplesPath = (documentsPath as NSString).appendingPathComponent("Examples")
        let examplesURL = URL(fileURLWithPath: examplesPath)
        try? FileManager.default.createDirectory(atPath: examplesPath,
                                                 withIntermediateDirectories: true,
                                                 attributes: nil)

        var resourceValues = URLResourceValues()
        resourceValues.isExcludedFromBackup = true
        try? (examplesURL as NSURL).setResourceValue(true, forKey: .isExcludedFromBackupKey)
    }
}
