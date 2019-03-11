//
//  SplashDocument.swift
//  splash
//
//  Created by Gonzo Fialho on 03/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class SplashDocument: UIDocument {
    enum ExecutionError: LocalizedError {
        case saveError
        case compilationError
        case shortcutsNotFound

        var errorDescription: String? {
            switch self {
            case .saveError: return "Unknown error when saving file"
            case .compilationError: return "Compilatio error"
            case .shortcutsNotFound: return "Shortctus app not found."
            }
        }
    }

    /// Just the last path component with extension
    var fileName: String {
        return (self.fileURL.path as NSString)
            .lastPathComponent
    }

    /// File name without extension
    var fileTitle: String {
        return (fileName as NSString)
            .deletingPathExtension
    }

    var string = String() {
        didSet {
            string.formatForCode()
        }
    }

    override func contents(forType typeName: String) throws -> Any {
        let data = string.data(using: .utf8)!
        return data
    }

    override func load(fromContents contents: Any, ofType typeName: String?) throws {
        guard let data = contents as? Data else {return}
        self.string = String(data: data, encoding: .utf8)!
    }

    func compileAndRun(completion: @escaping (ExecutionError?) -> Void) {
        save(to: fileURL,
             for: .forOverwriting) { [unowned self] (completed) in
                guard completed else {
                    completion(.saveError)
                    return
                }

                let tempDirectoryPath = NSTemporaryDirectory()
                let shortcutPath = (tempDirectoryPath as NSString).appendingPathComponent("temp.shortcut")
                let parseError = parse(self.fileURL.path, shortcutPath)
                if parseError != 0 {
                    completion(.compilationError)
                    return
                }

                completion(self.run(fromPath: shortcutPath))
        }
    }

    func run(fromPath path: String) -> ExecutionError? {
        let nsDictionary = NSDictionary(contentsOfFile: path)!
        // swiftlint:disable:next force_try
        let data = try! PropertyListSerialization.data(fromPropertyList: nsDictionary,
                                                       format: .binary,
                                                       options: 0)

        let b64 = "data:text/shortcut;base64," + data.base64EncodedString()
        var urlComponents = URLComponents(string: "shortcuts://import-shortcut")!
        let queryItems = [URLQueryItem(name: "name", value: fileTitle),
                          URLQueryItem(name: "url", value: b64)]
        urlComponents.queryItems = queryItems
        let url = urlComponents.url!
        if UIApplication.shared.canOpenURL(url) {
            UIApplication.shared.open(url,
                                      options: [:],
                                      completionHandler: nil)
        } else {
            return .shortcutsNotFound
        }
        return nil
    }
}

private extension String {
    mutating func formatForCode() {
        convertToLF()
        appendTrailingNewLineIfNeeded()
    }

    private mutating func convertToLF() {
        self = replacingOccurrences(of: "\r\n", with: "\n")
        self = replacingOccurrences(of: "\r", with: "")
    }

    private mutating func appendTrailingNewLineIfNeeded() {
        if !hasSuffix("\n") {
            append("\n")
        }
    }
}
