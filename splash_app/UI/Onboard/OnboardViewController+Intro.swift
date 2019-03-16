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

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            backgroundColor = .clear
        }
    }
}
