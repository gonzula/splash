//
//  CodeAccessoryView.swift
//  splash
//
//  Created by Gonzo Fialho on 04/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

protocol CodeAccessoryViewDelegate: class {
    func insertAutoCode(ofKind kind: CodeAccessoryView.CodeButtonKind)
}

class CodeAccessoryView: UIView {
    enum CodeButtonKind: CaseIterable {
        case curlyBrackets
        case parenthesis
        case quotes

        case comment

        case attribution
        case lessThan
        case greaterThan
        case equal

        case keywordIf
        case keywordElse
        case keywordElseIf

        var buttonTitle: String {
            switch self {
            case .curlyBrackets: return "{ }"
            case .parenthesis: return "( )"
            case .quotes: return "\" \""
            case .comment: return "#"
            case .attribution: return ":="
            case .lessThan: return "<"
            case .greaterThan: return ">"
            case .equal: return "=="
            case .keywordIf: return "if"
            case .keywordElse: return "else"
            case .keywordElseIf: return "else if"
            }
        }

        var needsSpaceBefore: Bool {
            switch self {
            case .parenthesis: return false
            case .curlyBrackets,
                 .quotes,
                 .comment,
                 .attribution,
                 .lessThan,
                 .greaterThan,
                 .equal,
                 .keywordIf,
                 .keywordElse,
                 .keywordElseIf: return true
            }
        }

        var needsSpaceAfter: Bool {
            switch self {
            case .parenthesis,
                 .quotes,
                 .curlyBrackets: return false
            case .attribution,
                 .lessThan,
                 .comment,
                 .greaterThan,
                 .equal,
                 .keywordIf,
                 .keywordElse,
                 .keywordElseIf: return true
            }
        }

        var needsCurlyBraceAfter: Bool {
            switch self {
            case .keywordElse: return true
            case .curlyBrackets,
                 .quotes,
                 .parenthesis,
                 .comment,
                 .attribution,
                 .lessThan,
                 .greaterThan,
                 .equal,
                 .keywordIf,
                 .keywordElseIf: return false
            }
        }

        var needsNewLineAfter: Bool {
            switch self {
            case .curlyBrackets: return true
            case .parenthesis,
                 .quotes,
                 .attribution,
                 .lessThan,
                 .comment,
                 .greaterThan,
                 .equal,
                 .keywordIf,
                 .keywordElse,
                 .keywordElseIf: return false
            }
        }

        var textToAdd: String {
            switch self {
            case .quotes: return "\"\""
            case .curlyBrackets: return "{"
            case .parenthesis: return "()"
            case .attribution,
                 .lessThan,
                 .greaterThan,
                 .comment,
                 .equal,
                 .keywordIf,
                 .keywordElse,
                 .keywordElseIf: return buttonTitle
            }
        }

        var offsetAfterInsertion: Int {
            switch self {
            case .parenthesis,
                 .quotes: return -1
            case .curlyBrackets,
                 .attribution,
                 .lessThan,
                 .greaterThan,
                 .comment,
                 .equal,
                 .keywordIf,
                 .keywordElse,
                 .keywordElseIf: return 0
            }
        }
    }

    let scrollView = UIScrollView()
    let stackView = UIStackView()

    unowned let delegate: CodeAccessoryViewDelegate

    init(delegate: CodeAccessoryViewDelegate) {
        self.delegate = delegate
        super.init(frame: CGRect(origin: .zero, size: CGSize(width: UIScreen.main.bounds.width, height: 44)))
        autoresizingMask = .flexibleWidth
        setup()
    }

    required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

    private func setup() {
        backgroundColor = UIColor(red: 209/255, green: 212/255, blue: 216/255, alpha: 1.0)
        scrollView.setupForAutoLayout(in: self)
        scrollView.pinToSuperview()
        scrollView.setupForHorizontalScrollOnly()

        stackView.setupForAutoLayout(in: scrollView)
        stackView.pinToSuperview(withInset: UIEdgeInsets(top: 0, left: 8, bottom: 0, right: 8))

        stackView.axis = .horizontal
        stackView.alignment = .center
        stackView.spacing = 20
        stackView.distribution = .equalSpacing

        CodeButtonKind.allCases
            .map(AutoCodeButton.init(codeButtonKind: ))
            .forEach(stackView.addArrangedSubview(_:))

        stackView.arrangedSubviews.compactMap {$0 as? AutoCodeButton}.forEach {
            $0.addTarget(self,
                         action: #selector(buttonTouched(sender:)),
                         for: .touchUpInside)
        }

        let line = UIView()
        line.setupForAutoLayout(in: self)
        line.leftAnchor.constraint(equalTo: leftAnchor).activate()
        line.rightAnchor.constraint(equalTo: rightAnchor).activate()
        line.bottomAnchor.constraint(equalTo: bottomAnchor).activate()
        line.heightAnchor.constraint(equalToConstant: 1).activate()
        line.backgroundColor = UIColor(red: 180/255, green: 188/255, blue: 199/255, alpha: 1)
    }

    // MARK: - UserInteraction

    @objc func buttonTouched(sender: AutoCodeButton) {
        delegate.insertAutoCode(ofKind: sender.codeButtonKind)
    }
}

extension CodeAccessoryView {
    class AutoCodeButton: UIButton {
        let codeButtonKind: CodeButtonKind

        init(codeButtonKind: CodeButtonKind) {
            self.codeButtonKind = codeButtonKind

            super.init(frame: .zero)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

        private func setup() {
            let attributedString = NSAttributedString(string: codeButtonKind.buttonTitle, attributes: [
                .font: UIFont(name: "Menlo", size: UIFont.systemFontSize)!,
                .foregroundColor: UIColor.black
                ])
            setAttributedTitle(attributedString, for: .normal)

            backgroundColor = .lightGray
            contentEdgeInsets = UIEdgeInsets(top: 8, left: 8, bottom: 8, right: 8)
            layer.cornerRadius = 4
        }
    }
}
