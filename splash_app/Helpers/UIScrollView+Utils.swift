//
//  UIScrollView+Utils.swift
//  splash
//
//  Created by Gonzo Fialho on 04/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension UIScrollView {
    func setupForVerticalScrollOnly() {
        let view = UIView()
        view.setupForAutoLayout(in: self)
        view.backgroundColor = .clear
        view.topAnchor.constraint(equalTo: topAnchor).activate()
        view.leftAnchor.constraint(equalTo: leftAnchor).activate()
        view.rightAnchor.constraint(equalTo: rightAnchor).activate()
        view.heightAnchor.constraint(equalToConstant: 1).activate()
        view.widthAnchor.constraint(equalTo: widthAnchor).activate()
        view.setRequiredSize()
    }

    func setupForHorizontalScrollOnly() {
        let view = UIView()
        view.setupForAutoLayout(in: self)
        view.backgroundColor = .clear
        view.topAnchor.constraint(equalTo: topAnchor).activate()
        view.leftAnchor.constraint(equalTo: leftAnchor).activate()
        view.bottomAnchor.constraint(equalTo: bottomAnchor).activate()
        view.widthAnchor.constraint(equalToConstant: 1).activate()
        view.heightAnchor.constraint(equalTo: heightAnchor).activate()
        view.setRequiredSize()
    }
}
