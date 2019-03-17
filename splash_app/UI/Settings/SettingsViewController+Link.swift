//
//  SettingsViewController+LinkCell.swift
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

        var cellIdentifier: String {return "link cell"}
        var cellClass: UITableViewCell.Type {return LinkCell.self}

        func configure(_ cell: UITableViewCell, at indexPath: IndexPath, in viewController: SettingsViewController) {
            cell.textLabel?.text = name
        }

        func action(for indexPath: IndexPath) -> ((SettingsViewController) -> Void)? {
            return { _ in
                UIApplication.shared.open(self.url,
                                          options: [:],
                                          completionHandler: nil)
            }
        }
    }
}

extension SettingsViewController {
    class LinkCell: UITableViewCell {

        override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
            super.init(style: .default, reuseIdentifier: reuseIdentifier)
        }

        required init?(coder aDecoder: NSCoder) {fatalError("init(coder:) has not been implemented")}
    }
}

extension SettingsViewController.LinkCell: AppearanceAdjustable {
    func setupAppearance() {
        let theme = ThemeManager.shared.theme
        backgroundColor = theme.tableViewCellBackgroundColor
        textLabel?.textColor = theme.textColor
    }
}
