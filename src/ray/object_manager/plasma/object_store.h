// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include "absl/container/flat_hash_map.h"
#include "ray/object_manager/plasma/common.h"
#include "ray/object_manager/plasma/plasma.h"

namespace plasma {

// ObjectStore tracks object created over memory allocation.
// It provides APIs to create/get/seal/delete objects.
class ObjectStore {
 public:
  ObjectStore();

  // Create a new object given allocation, object_info and source.
  // Abort if such object with the same object_id already exists.
  const LocalObject *CreateObject(Allocation allocation,
                                  const ray::ObjectInfo &object_info,
                                  plasma::flatbuf::ObjectSource source);

  // Get a local object by object_id.
  // Returns nullptr if such object doesn't exist.
  const LocalObject *GetObject(const ObjectID &object_id) const;

  // Seal an unsealed object.
  // Abort if such object doesn't exist or object has already been sealed.
  const LocalObject *SealObject(const ObjectID &object_id);

  // Delete an existing object.
  // Abort if such object doesn't exist.
  Allocation DeleteObject(const ObjectID &object_id);

  int64_t GetNumBytesCreatedTotal() const;

  int64_t GetNumBytesUnsealed() const;

  int64_t GetNumObjectsUnsealed() const;

  void GetDebugDump(std::stringstream &buffer) const;

 private:
  LocalObject *GetMutableObject(const ObjectID &object_id);

  /// Mapping from ObjectIDs to information about the object.
  absl::flat_hash_map<ObjectID, std::unique_ptr<LocalObject>> object_table_;

  /// Total number of bytes allocated to objects that are created but not yet
  /// sealed.
  int64_t num_bytes_unsealed_ = 0;

  /// Number of objects that are created but not sealed.
  int64_t num_objects_unsealed_ = 0;

  /// A running total of the objects that have ever been created on this node.
  int64_t num_bytes_created_total_ = 0;
};

}  // namespace plasma
