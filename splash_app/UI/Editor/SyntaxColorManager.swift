//
//  SyntaxColorManager.swift
//  splash
//
//  Created by Gonzo Fialho on 04/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class SyntaxColorManager {
    enum TokenKind {
        case comment
        case keyword
        case functionCall
        case identifier
        case number
        case string

        var attributes: [NSAttributedString.Key: Any] {
            switch self {
            case .comment: return [.foregroundColor: UIColor(hex: 0x536579),
                                   .font: UIFont(name: "Menlo-Italic", size: UIFont.systemFontSize)!]
            case .keyword: return [.foregroundColor: UIColor(hex: 0x9B2393),
                                   .font: UIFont(name: "Menlo-Bold", size: UIFont.systemFontSize)!]
            case .functionCall: return [.foregroundColor: UIColor(hex: 0x3900A0)]
            case .identifier: return [.foregroundColor: UIColor(hex: 0x000000)]
            case .number: return [.foregroundColor: UIColor(hex: 0x1C00CF)]
            case .string: return [.foregroundColor: UIColor(hex: 0xC41A16)]
            }
        }
    }

    let patterns: [(String, TokenKind)] = [
        ("#.*?(?:$|\\n)", .comment),
        ("[a-zA-Z_][a-zA-Z_0-9]*(?=\\s*\\()", .functionCall),
        ("[a-zA-Z_][a-zA-Z_0-9]*", .identifier),
        ("[0-9]+(?:\\.[0-9]+)?", .number),
        ("\"(?:[^\"\\\\]|\\\\.)*\"|\'(?:[^\'\\\\]|\\\\.)*\'", .string)
        ]

    lazy var pattern: String = patterns
        .map {"(\($0.0))"}
        .joined(separator: "|")

    func colorize(_ input: String) -> NSAttributedString {
        let attributedString = NSMutableAttributedString(string: input, attributes: [
            .font: UIFont(name: "Menlo", size: UIFont.systemFontSize)!,
            .foregroundColor: UIColor.black
            ])

        let regex = try! NSRegularExpression(pattern: pattern, options: []) // swiftlint:disable:this force_try
        let results = regex.matches(in: input, options: [], range: NSRange(location: 0, length: input.count))

        for result in results {
            let kind = patterns[result.matchId].1

            let attributes: [NSAttributedString.Key: Any]
            if kind == .identifier {
                let value = (input as NSString).substring(with: result.range)
                    .trimmingCharacters(in: .whitespacesAndNewlines)
                if isKeyword(identifier: value) {
                    attributes = TokenKind.keyword.attributes
                } else {
                    attributes = kind.attributes
                }
            } else {
                attributes = kind.attributes
            }

            attributedString.addAttributes(attributes, range: result.range)
        }

        return attributedString
    }

    func isKeyword(identifier: String) -> Bool {
        let keywords = ["if", "else"]
        return keywords.contains(identifier)
    }
}

fileprivate extension NSTextCheckingResult {
    var matchId: Int {
        return (1..<numberOfRanges).filter {range(at: $0).length != 0}.first! - 1
    }
}
