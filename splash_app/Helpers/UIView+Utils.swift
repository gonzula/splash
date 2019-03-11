//
//  UIView+Utils.swift
//  splash
//
//  Created by Gonzo Fialho on 04/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension UIView {
    func pin(to other: UIView, withInset inset: UIEdgeInsets = .zero) {
        topAnchor.constraint(equalTo: other.topAnchor, constant: inset.top).isActive = true
        leftAnchor.constraint(equalTo: other.leftAnchor, constant: inset.left).isActive = true
        rightAnchor.constraint(equalTo: other.rightAnchor, constant: -inset.right).isActive = true
        bottomAnchor.constraint(equalTo: other.bottomAnchor, constant: -inset.bottom).isActive = true
    }

    func pinToSuperview(withInset inset: UIEdgeInsets = .zero) {
        pin(to: superview!, withInset: inset)
    }

    func placeAtCenter() {
        centerXAnchor.constraint(equalTo: superview!.centerXAnchor).isActive = true
        centerYAnchor.constraint(equalTo: superview!.centerYAnchor).isActive = true
    }

    func setupForAutoLayout(in superview: UIView) {
        self.translatesAutoresizingMaskIntoConstraints = false
        superview.addSubview(self)
    }

    func setRequiredSize() {
        setContentHuggingPriority(.required, for: .horizontal)
        setContentCompressionResistancePriority(.required, for: .horizontal)
        setContentHuggingPriority(.required, for: .vertical)
        setContentCompressionResistancePriority(.required, for: .vertical)
    }

    func toImage() -> UIImage? {
        UIGraphicsBeginImageContextWithOptions(bounds.size, true, 1.0)
        guard let context = UIGraphicsGetCurrentContext() else {return nil}
        defer { UIGraphicsEndImageContext() }
        layer.render(in: context)
        guard let image = UIGraphicsGetImageFromCurrentImageContext() else {return nil}
        return image
    }
}
