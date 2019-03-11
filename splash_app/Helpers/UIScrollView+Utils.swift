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
        view.topAnchor.constraint(equalTo: topAnchor).isActive = true
        view.leftAnchor.constraint(equalTo: leftAnchor).isActive = true
        view.rightAnchor.constraint(equalTo: rightAnchor).isActive = true
        view.heightAnchor.constraint(equalToConstant: 1).isActive = true
        view.widthAnchor.constraint(equalTo: widthAnchor).isActive = true
        view.setRequiredSize()
    }

    func setupForHorizontalScrollOnly() {
        let view = UIView()
        view.setupForAutoLayout(in: self)
        view.backgroundColor = .clear
        view.topAnchor.constraint(equalTo: topAnchor).isActive = true
        view.leftAnchor.constraint(equalTo: leftAnchor).isActive = true
        view.bottomAnchor.constraint(equalTo: bottomAnchor).isActive = true
        view.widthAnchor.constraint(equalToConstant: 1).isActive = true
        view.heightAnchor.constraint(equalTo: heightAnchor).isActive = true
        view.setRequiredSize()
    }
}
