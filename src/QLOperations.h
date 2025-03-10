/*
 * QLOperations.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2018 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _QL_OPERATIONS_H
#define _QL_OPERATIONS_H
#pragma once

#include "QLContext.h"

struct IUpdateOp {
	virtual void addref() = 0;
	virtual void delref() = 0;
	virtual Future<Void> update(Reference<IReadWriteContext> document) = 0;
	virtual std::string describe() = 0;
};

template <class Concrete>
struct ConcreteUpdateOp : IUpdateOp, ReferenceCounted<Concrete>, FastAllocated<Concrete> {
	void addref() override { ReferenceCounted<Concrete>::addref(); }
	void delref() override { ReferenceCounted<Concrete>::delref(); }
};

struct IInsertOp {
	virtual void addref() = 0;
	virtual void delref() = 0;
	virtual Future<Reference<IReadWriteContext>> insert(Reference<CollectionContext>) = 0;
	virtual std::string describe() = 0;
};

template <class Concrete>
struct ConcreteInsertOp : IInsertOp, ReferenceCounted<Concrete>, FastAllocated<Concrete> {
	void addref() override { ReferenceCounted<Concrete>::addref(); }
	void delref() override { ReferenceCounted<Concrete>::delref(); }
};

struct DeleteDocument : ConcreteUpdateOp<DeleteDocument> {
	Future<Void> update(Reference<IReadWriteContext> document) override {
		document->clearDescendants();
		document->clearRoot();
		return Future<Void>(Void());
	}
	std::string describe() override { return DocLayerConstants::DESCRIBE_DELETE_DOC; }
};

#endif