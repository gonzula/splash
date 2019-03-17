//
//  SettingsViewController+ThemeCell.swift
//  splash
//
//  Created by Gonzo Fialho on 17/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension SettingsViewController {
    struct Theme: CellConfigurator {
        var cellIdentifier: String {return "theme cell"}
        var cellClass: UITableViewCell.Type {return ThemeCell.self}

        func configure(_ cell: UITableViewCell, at indexPath: IndexPath, in viewController: SettingsViewController) {

        }

        func action(for indexPath: IndexPath) -> ((SettingsViewController) -> Void)? {return nil}
    }
}

extension SettingsViewController {
    class ThemeCell: UITableViewCell {
        let label = UILabel()
        let themeControl = ThemeControl()

        override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
            super.init(style: style, reuseIdentifier: reuseIdentifier)

            setup()
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}

        private func setup() {
            setupLabel()
            setupThemeControl()
        }

        private func setupLabel() {
            label.setupForAutoLayout(in: contentView)
            label.centerYAnchor.constraint(equalTo: contentView.centerYAnchor).activate()
            label.leftAnchor.constraint(equalTo: contentView.leftAnchor, constant: 20).activate()
            label.text = "Theme"
        }

        private func setupThemeControl() {
            themeControl.setupForAutoLayout(in: contentView)

            themeControl.rightAnchor.constraint(equalTo: contentView.rightAnchor, constant: -20).activate()
            themeControl.centerYAnchor.constraint(equalTo: contentView.centerYAnchor).activate()
        }
    }
}
