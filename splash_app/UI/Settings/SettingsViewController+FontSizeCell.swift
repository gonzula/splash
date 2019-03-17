//
//  SettingsViewController+ThemeCell.swift
//  splash
//
//  Created by Gonzo Fialho on 17/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension SettingsViewController {
    struct FontSize: CellConfigurator {
        var cellIdentifier: String {return "font size cell"}
        var cellClass: UITableViewCell.Type {return FontSizeCell.self}

        func configure(_ cell: UITableViewCell, at indexPath: IndexPath, in viewController: SettingsViewController) {
        }

        func action(for indexPath: IndexPath) -> ((SettingsViewController) -> Void)? {return nil}
    }
}

extension SettingsViewController {
    class FontSizeCell: UITableViewCell {
        let label = UILabel()
        let stepper = UIStepper()

        override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
            super.init(style: style, reuseIdentifier: reuseIdentifier)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

        private func setup() {
            setupLabel()
            setupStepper()
        }

        private func setupLabel() {
            label.setupForAutoLayout(in: contentView)
            label.centerYAnchor.constraint(equalTo: contentView.centerYAnchor).activate()
            label.leftAnchor.constraint(equalTo: contentView.leftAnchor, constant: 20).activate()
            label.text = "Code font size:"
        }

        private func setupStepper() {
            stepper.setupForAutoLayout(in: contentView)

            stepper.rightAnchor.constraint(equalTo: contentView.rightAnchor, constant: -20).activate()
            stepper.centerYAnchor.constraint(equalTo: contentView.centerYAnchor).activate()
        }
    }
}

extension SettingsViewController.FontSizeCell: AppearanceAdjustable {
    func setupAppearance() {
        let theme = ThemeManager.shared.theme
        backgroundColor = theme.tableViewCellBackgroundColor
        label.textColor = theme.textColor
    }
}
