//
//  SettingsViewController+DefaultCell.swift
//  splash
//
//  Created by Gonzo Fialho on 17/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

extension SettingsViewController {
    struct Link: CellConfigurator {

        let name: String
        let url: URL

        var cellIdentifier: String {return "default cell"}
        var cellClass: UITableViewCell.Type {return DefaultCell.self}

        func configure(_ cell: UITableViewCell, at indexPath: IndexPath, in viewController: SettingsViewController) {
            cell.textLabel?.text = name
        }

        func action(for indexPath: IndexPath) -> ((SettingsViewController) -> Void)? {
            return { viewController in
                viewController.openURL(self.url)
            }
        }
    }
}

extension SettingsViewController {
    struct CustomAction: CellConfigurator {

        let name: String
        let action: (SettingsViewController) -> Void

        var cellIdentifier: String {return "default cell"}
        var cellClass: UITableViewCell.Type {return DefaultCell.self}

        func configure(_ cell: UITableViewCell, at indexPath: IndexPath, in viewController: SettingsViewController) {
            cell.textLabel?.text = name
        }

        func action(for indexPath: IndexPath) -> ((SettingsViewController) -> Void)? {
            return action
        }
    }
}

extension SettingsViewController {
    class DefaultCell: UITableViewCell {

        override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
            super.init(style: .default, reuseIdentifier: reuseIdentifier)
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}
    }
}

extension SettingsViewController.DefaultCell: AppearanceAdjustable {
    func setupAppearance() {
        let theme = ThemeManager.shared.theme
        backgroundColor = theme.tableViewCellBackgroundColor
        textLabel?.textColor = theme.textColor
    }
}
