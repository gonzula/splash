//
//  OnboardViewController+Intro.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class IntroViewController: InsidePageViewController {
        let introView = IntroView()
        override func loadView() {
            super.loadView()

            title = "Welcome to Splash!"
            caption = """
            Splash is a programming language that helps you to create your (complex) shortcuts.
            """
            introView.setupForAutoLayout(in: contentView)
            introView.pinToSuperview()
        }
    }
}

extension OnboardViewController {
    class IntroView: UIView {

        let editorView = EditorView()
        let actionView = ActionView()

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

        private func setup() {
            setupEditorView()
            setupActionView()
        }

        private func setupEditorView() {
            editorView.setupForAutoLayout(in: self)

            editorView.topAnchor.constraint(equalTo: topAnchor).activate()
            editorView.leftAnchor.constraint(equalTo: leftAnchor).activate()
            editorView.rightAnchor.constraint(equalTo: rightAnchor).activate()
            editorView.heightAnchor.constraint(equalToConstant: 84).activate()

            editorView.text = """
            # The following code:
            ShowResult(\"Hello World\")

            # Will compile to the following shortcut:
            """
            editorView.colorizeText()
            editorView.isUserInteractionEnabled = false
        }

        private func setupActionView() {
            actionView.setupForAutoLayout(in: self)

            actionView.centerXAnchor.constraint(equalTo: centerXAnchor).activate()
            actionView.topAnchor.constraint(equalTo: editorView.bottomAnchor, constant: 8).activate()
        }
    }
}
