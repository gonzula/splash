//
//  Bundle+Resources.swift
//  splash
//
//  Created by Gonzo Fialho on 10/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import Foundation

extension Bundle {

    var shortVersion: String {
        return infoDictionary?["CFBundleShortVersionString"] as? String ?? "unknown"
    }

    var longVersion: String {
        return infoDictionary?["CFBundleVersion"] as? String ?? "unknown"
    }

    var fullVersion: String {return "\(shortVersion) (\(longVersion))"}

    func dataFromResource(fileName: String) -> Data {
        // swiftlint:disable:next force_try
        return try! Data(contentsOf: url(forResource: fileName, withExtension: nil)!)
    }

    func helpHtmlContent() -> String {
        let htmlData = dataFromResource(fileName: "Help.html")
        var htmlString = String(data: htmlData, encoding: .utf8)!
        htmlString = htmlString.replacingOccurrences(of: "<+APP_VERSION+>", with: fullVersion)
        return htmlString
    }
}
