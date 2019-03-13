//
//  UserDefaults+Key.swift
//  splash
//
//  Created by Gonzo Fialho on 12/03/19.
//  Copyright © 2019 Gonzo Fialho. All rights reserved.
//

import Foundation

public extension UserDefaults {
    public struct Domain: Equatable {
        public let name: String

        public init(name: String) {
            self.name = name
        }
    }

    public struct Key: Equatable {
        public let domain: Domain
        public let keyName: String

        public var rawValue: String {
            return "\(domain.name).\(keyName)"
        }

        public init(domain: Domain, keyName: String) {
            self.domain = domain
            self.keyName = keyName
        }
    }

    public func object(forKey key: Key) -> Any? {
        return object(forKey: key.rawValue)
    }

    public func url(forKey key: Key) -> URL? {
        return url(forKey: key.rawValue)
    }

    public func array(forKey key: Key) -> [Any]? {
        return array(forKey: key.rawValue)
    }

    public func dictionary(forKey key: Key) -> [String: Any]? {
        return dictionary(forKey: key.rawValue)
    }

    public func string(forKey key: Key) -> String? {
        return string(forKey: key.rawValue)
    }

    public func stringArray(forKey key: Key) -> [String]? {
        return stringArray(forKey: key.rawValue)
    }

    public func data(forKey key: Key) -> Data? {
        return data(forKey: key.rawValue)
    }

    public func bool(forKey key: Key) -> Bool {
        return bool(forKey: key.rawValue)
    }

    public func integer(forKey key: Key) -> Int {
        return integer(forKey: key.rawValue)
    }

    public func float(forKey key: Key) -> Float {
        return float(forKey: key.rawValue)
    }

    public func double(forKey key: Key) -> Double {
        return double(forKey: key.rawValue)
    }

    public func set(_ object: Any?, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    public func set(_ object: Float, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    public func set(_ object: Double, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    public func set(_ object: Int, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    public func set(_ object: Bool, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    public func set(_ object: URL?, forKey key: Key) {
        set(object, forKey: key.rawValue)
    }

    public func removeObject(forKey key: Key) {
        removeObject(forKey: key.rawValue)
    }
}
