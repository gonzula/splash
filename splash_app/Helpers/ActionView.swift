//
//  ActionView.swift
//  splash
//
//  Created by Gonzo Fialho on 16/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class ActionView: UIView {

    override init(frame: CGRect) {
        super.init(frame: frame)

        isOpaque = false
        layer.shadowColor = UIColor.black.cgColor
        layer.shadowRadius = 40
        layer.shadowOpacity = 0.2
    }

    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override var intrinsicContentSize: CGSize {return CGSize(width: 300, height: 335/2)}

    override func draw(_ rect: CGRect) {
        drawTopRect()
        drawBottomRect()
        drawLine()

        drawTitle()
        drawText()

        drawGear()
    }

    private func drawTopRect() {
        let topRect = UIBezierPath(roundedRect: CGRect(origin: .zero,
                                                       size: CGSize(width: bounds.width, height: 45)),
                                   byRoundingCorners: [.topLeft, .topRight],
                                   cornerRadii: CGSize(width: 13, height: 13))
        UIColor(red: 242/255, green: 244/255, blue: 246/255, alpha: 1.0).setFill()
        topRect.fill()
    }

    private func drawBottomRect() {
        let bottomRect = UIBezierPath(roundedRect: CGRect(x: 0,
                                                          y: 45,
                                                          width: bounds.width,
                                                          height: bounds.height - 45),
                                      byRoundingCorners: [.bottomLeft, .bottomRight],
                                      cornerRadii: CGSize(width: 13, height: 13))
        UIColor.white.setFill()
        bottomRect.fill()
    }

    private func drawLine() {
        let line = UIBezierPath()
        line.lineWidth = 0.5
        line.move(to: CGPoint(x: 0, y: 45))
        line.addLine(to: CGPoint(x: bounds.width, y: 45))
        UIColor(white: 210/255, alpha: 1.0).setStroke()
        line.stroke()
    }

    private func drawTitle() {
        let text = NSAttributedString(string: "Show Result", attributes: [
            .font: UIFont.systemFont(ofSize: 16, weight: .regular),
            .foregroundColor: UIColor.black
            ])
        text.draw(at: CGPoint(x: 44, y: 14))
    }

    private func drawText() {
        let text = NSAttributedString(string: "Hello World", attributes: [
            .font: UIFont.systemFont(ofSize: 16, weight: .light),
            .foregroundColor: UIColor.black
            ])
        text.draw(at: CGPoint(x: 16, y: 57))
    }

    private func drawGear() {
        var rect = CGRect(x: 8,
                          y: 8,
                          width: 29,
                          height: 29)
        let path = UIBezierPath(roundedRect: rect, cornerRadius: 7)
        UIColor(white: 141/255, alpha: 1.0).setFill()
        path.fill()

        rect = rect.insetBy(dx: 3, dy: 3)
        let gear = #imageLiteral(resourceName: "gear")
        gear.draw(in: rect)
    }
}
