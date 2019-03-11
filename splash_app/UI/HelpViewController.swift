//
//  HelpViewController.swift
//  splash
//
//  Created by Gonzo Fialho on 10/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import UIKit
import WebKit

class HelpNavigationController: UINavigationController {
    init() {
        super.init(rootViewController: HelpViewController())
    }

    override init(nibName nibNameOrNil: String?, bundle nibBundleOrNil: Bundle?) {
        super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
    }

    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

class HelpViewController: UIViewController {

    let webView = WKWebView()

    override func loadView() {
        view = webView

        setupWebView()
        navigationItem.leftBarButtonItem = UIBarButtonItem(barButtonSystemItem: .cancel,
                                                           target: self,
                                                           action: #selector(dismiss(sender:)))
    }

    fileprivate func setupWebView() {
        webView.navigationDelegate = self
        webView.loadHTMLString(Bundle.main.helpHtmlContent(), baseURL: nil)
    }

    // MARK: - User Interaction

    @objc func dismiss(sender: UIBarButtonItem) {
        self.dismiss(animated: true, completion: nil)
    }
}

extension HelpViewController: WKNavigationDelegate {
    func webView(_ webView: WKWebView,
                 decidePolicyFor navigationAction: WKNavigationAction,
                 decisionHandler: @escaping (WKNavigationActionPolicy) -> Void) {
        let url = navigationAction.request.url
        if url == URL(string: "about:blank") {
            decisionHandler(.allow)
        } else {
            decisionHandler(.cancel)
        }

        if url == URL(string: "https://github.com/gonzula/splash") {
            UIApplication.shared.open(url!,
                                      options: [:],
                                      completionHandler: nil)
        }
        if url == URL(string: "restore_examples") {
            (UIApplication.shared.delegate as? AppDelegate)?.installExamples()
        }
    }
}
