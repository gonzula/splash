//
//  OnboardViewController+FixedContent.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class FixedContent: UIView {
        let pageControl = UIPageControl()

        override func hitTest(_ point: CGPoint, with event: UIEvent?) -> UIView? {
            return pageControl.hitTest(self.convert(point, to: pageControl), with: event)
        }

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            pageControl.setupForAutoLayout(in: self)
            pageControl.bottomAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor,
                                                constant: -20).activate()
            pageControl.centerXAnchor.constraint(equalTo: safeAreaLayoutGuide.centerXAnchor).activate()

            pageControl.pageIndicatorTintColor = .lightGray
            pageControl.currentPageIndicatorTintColor = .black
        }
    }
}
