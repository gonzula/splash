//
//  BrowserDelegate.swift
//  splash
//
//  Created by Gonzo Fialho on 17/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit

class BrowserDelegate: NSObject, UIDocumentBrowserViewControllerDelegate {
    func documentBrowser(_ controller: UIDocumentBrowserViewController, didPickDocumentURLs documentURLs: [URL]) {
        guard let url = documentURLs.first else {return}
        guard let viewController = controller as? ViewController else {return}

        viewController.presentEditor(withURL: url)
    }

    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         didRequestDocumentCreationWithHandler importHandler:
        @escaping (URL?, UIDocumentBrowserViewController.ImportMode) -> Void) {
        guard let viewController = controller as? ViewController else {return}

        let alertController = UIAlertController(title: "New Script",
                                                message: nil,
                                                preferredStyle: .alert)
        alertController.addTextField { textField in
            textField.autocapitalizationType = .none
            textField.returnKeyType = .done
            textField.autocorrectionType = .no
            textField.spellCheckingType = .no
            textField.smartDashesType = .no
            textField.smartQuotesType = .no
            textField.placeholder = "File name"
        }
        alertController.addAction(
            UIAlertAction(title: "Create",
                          style: .default,
                          handler: { _ in
                            let url = self.createNewFile(
                                named: alertController.textFields?.first?.text ?? "")
                            if let url = url {
                                importHandler(url, .move)
                            } else {
                                importHandler(nil, .none)
                            }
            }))
        alertController.addAction(UIAlertAction(title: "Cancel",
                                                style: .cancel,
                                                handler: { _ in
                                                    importHandler(nil, .none)
        }))
        viewController.present(alertController, animated: true, completion: nil)
    }

    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         didImportDocumentAt sourceURL: URL,
                         toDestinationURL destinationURL: URL) {
        guard let viewController = controller as? ViewController else {return}
        viewController.presentEditor(withURL: destinationURL)
    }

    func documentBrowser(_ controller: UIDocumentBrowserViewController,
                         failedToImportDocumentAt documentURL: URL,
                         error: Error?) {
        guard let viewController = controller as? ViewController else {return}
        let alertController: UIAlertController
        if let error = error {
            alertController = UIAlertController(error: error)
        } else {
            alertController = UIAlertController(title: "Error",
                                                message: "Unknown error occurred.",
                                                preferredStyle: .alert)
        }
        viewController.present(alertController,
                               animated: true,
                               completion: nil)
    }

    fileprivate func createNewFile(named name: String) -> URL? {
        let tempDirectoryPath = NSTemporaryDirectory()
        var name = name.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !name.isEmpty else {return nil}
        if !name.lowercased().hasSuffix(".splash") {
            name += ".splash"
        }
        let filePath = (tempDirectoryPath as NSString).appendingPathComponent("\(name)")
        let url = URL(fileURLWithPath: filePath)
        do {
            try Data().write(to: url)
            return url
        } catch {
            return nil
        }
    }
}
