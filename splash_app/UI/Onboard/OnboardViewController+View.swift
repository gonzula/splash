//
//  OnboardViewController+FixedContent.swift
//  splash
//
//  Created by Gonzo Fialho on 14/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension OnboardViewController {
    class FixedView: UIView {
        let pageControl = UIPageControl()
        let bottomView = UIVisualEffectView(effect: UIBlurEffect(style: .extraLight))

        override func hitTest(_ point: CGPoint, with event: UIEvent?) -> UIView? {
            return bottomView.hitTest(self.convert(point, to: bottomView), with: event)
        }

        override init(frame: CGRect) {
            super.init(frame: frame)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {
            fatalError("init(coder:) has not been implemented")
        }

        private func setup() {
            setupBottomView()
            setupPageControl()
        }

        private func setupBottomView() {
            bottomView.setupForAutoLayout(in: self)

            bottomView.leftAnchor.constraint(equalTo: leftAnchor).activate()
            bottomView.rightAnchor.constraint(equalTo: rightAnchor).activate()
            bottomView.bottomAnchor.constraint(equalTo: bottomAnchor).activate()
            bottomView.topAnchor.constraint(equalTo: safeAreaLayoutGuide.bottomAnchor, constant: -64).activate()
        }

        private func setupPageControl() {
            pageControl.setupForAutoLayout(in: bottomView.contentView)
            pageControl.bottomAnchor.constraint(equalTo: bottomView.safeAreaLayoutGuide.bottomAnchor,
                                                constant: -20).activate()
            pageControl.centerXAnchor.constraint(equalTo: bottomView.safeAreaLayoutGuide.centerXAnchor).activate()

            pageControl.pageIndicatorTintColor = .lightGray
            pageControl.currentPageIndicatorTintColor = .black
            pageControl.addTarget(nil,
                                  action: #selector(OnboardViewController.pageControlChanged(sender:)),
                                  for: .valueChanged)
        }
    }
}
