//
//  EditorView.swift
//  splash
//
//  Created by Gonzo Fialho on 10/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class EditorView: UITextView {

    var forcedTheme: ThemeManager.Theme? {
        didSet {
            setupAppearence()
        }
    }

    private var theme: ThemeManager.Theme {return forcedTheme ?? ThemeManager.shared.theme}

    var observers = [Any]()

    override init(frame: CGRect, textContainer: NSTextContainer?) {
        super.init(frame: frame, textContainer: textContainer)

        setup()
    }

    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    deinit {
        observers.forEach(NotificationCenter.default.removeObserver)
    }

    private func setup() {
        observers.append(  // swiftlint:disable:next discarded_notification_center_observer
            NotificationCenter.default.addObserver(
            forName: .themeChanged, object: nil, queue: nil) {[weak self] _ in
                self?.setupAppearence()
        })

        setupAppearence()

        inputAccessoryView = CodeAccessoryView(delegate: self)
        font = UIFont(name: "Menlo", size: UIFont.systemFontSize)
        autocapitalizationType = .none
        returnKeyType = .default
        autocorrectionType = .no
        spellCheckingType = .no
        smartDashesType = .no
        smartQuotesType = .no
        delegate = self
        keyboardType = .asciiCapable
        keyboardDismissMode = .interactive
    }

    private func setupAppearence() {
        self.colorizeText()
        self.backgroundColor = self.theme.backgroundColor
        self.keyboardAppearance = self.theme.keyboardAppearance
    }

    func colorizeText() {
        keepLocation { () -> Int? in
            attributedText = SyntaxColorizer().colorize(text)
            return nil
        }
    }

    @discardableResult
    func insertNewLine(at range: NSRange) -> Int {
        let indentationLevel = self.text.indentationLevel(at: range.location)
        let indentationUnit = String(repeating: " ", count: 4)
        let indentedNewLine = "\n" + String(repeating: indentationUnit, count: indentationLevel)
        let newLocation = keepLocation { () -> Int? in
            text = (self.text as NSString).replacingCharacters(in: range, with: indentedNewLine)
            return indentedNewLine.count
        }
        return newLocation
    }

    func insertClosingBracket(at location: Int) {
        let indentationLevel = max(self.text.indentationLevel(at: location) - 1, 0)
        let indentationUnit = String(repeating: " ", count: 4)
        let indentedNewLine = "\n" + String(repeating: indentationUnit, count: indentationLevel) + "}"
        let range = NSRange(location: location, length: 0)
        keepLocation { () -> Int? in
            text = (self.text as NSString).replacingCharacters(in: range, with: indentedNewLine)
            return nil
        }
    }

    /// Perform changes in *changes* and places the caret at the same location offseted by `changes` return value
    @discardableResult
    func keepLocation(after changes: () -> Int?) -> Int {
        let location = selectedRange.location
        let offset = changes() ?? 0
        let newLocation = location + offset
        selectedRange = NSRange(location: newLocation, length: 0)
        return newLocation
    }
}

extension EditorView: UITextViewDelegate {
    func textViewDidChange(_ textView: UITextView) {
        colorizeText()
    }

    // https://stackoverflow.com/a/6113117
    func textView(_ textView: UITextView,
                  shouldChangeTextIn range: NSRange,
                  replacementText: String) -> Bool {
        if range.location > 0, !replacementText.isEmpty {
            let whitespace = CharacterSet.whitespaces

            let start = replacementText.unicodeScalars.startIndex
            let location = textView.text.unicodeScalars.index(textView.text.unicodeScalars.startIndex,
                                                              offsetBy: range.location - 1)
            if whitespace.contains(replacementText.unicodeScalars[start]),
                whitespace.contains(textView.text.unicodeScalars[location]) {
                textView.text = (textView.text as NSString).replacingCharacters(in: range, with: " ")
                textView.selectedRange = NSRange(location: range.location + 1, length: 0)
                colorizeText()
                return false
            }
        }

        if replacementText == "\n" {
            insertNewLine(at: range)
            colorizeText()
            return false
        }

        return true
    }
}

extension EditorView: CodeAccessoryViewDelegate {
    func insertAutoCode(ofKind kind: CodeAccessoryView.CodeButtonKind) {
        var textToAdd = kind.textToAdd
        let range = selectedRange
        let location = range.location
        var text = self.text as NSString
        if kind.needsSpaceBefore, !range.isAtBegining {
            let previousChar = text.character(at: range.location - 1)
            var characterSet = CharacterSet.whitespacesAndNewlines
            characterSet.insert(charactersIn: "(")
            let previousCharWasSpace = characterSet.contains(Unicode.Scalar(previousChar)!)
            if !previousCharWasSpace {
                textToAdd = " " + textToAdd
            }
        }

        if kind.needsSpaceAfter {
            textToAdd += " "
        }

        text = text.replacingCharacters(in: range, with: textToAdd) as NSString
        self.text = text as String
        selectedRange = NSRange(location: location + textToAdd.count + kind.offsetAfterInsertion, length: 0)

        if kind.needsNewLineAfter {
            let newLocation = insertNewLine(at: selectedRange)
            if kind.textToAdd == "{" {
                insertClosingBracket(at: newLocation)
            }
        }

        if kind.needsCurlyBraceAfter {
            insertAutoCode(ofKind: .curlyBrackets)
            return
        }

        colorizeText()
    }
}

private extension String {
    func indentationLevel(at location: Int) -> Int {
        var indentationLevel = 0
        var currentStringDelimiter: Character?
        var isEscaped = false
        for (idx, char) in self.enumerated() {
            guard idx < location else {break}
            if let delimiter = currentStringDelimiter {
                if isEscaped {
                    isEscaped = false
                    continue
                } else {
                    if char == delimiter {
                        currentStringDelimiter = nil
                        continue
                    }
                }
            } else {
                switch char {
                case "{": indentationLevel += 1
                case "}": indentationLevel -= 1
                case "'", "\"": currentStringDelimiter = char
                default: break
                }
                continue
            }
        }
        return indentationLevel
    }
}
