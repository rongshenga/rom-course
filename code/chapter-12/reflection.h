/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_RUNTIME_REFLECTION_H_
#define ART_RUNTIME_REFLECTION_H_

#include "base/enums.h"
#include "base/locks.h"
#include "dex/primitive.h"
#include "jni.h"
#include "obj_ptr.h"

namespace art {
namespace mirror {
class Class;
class Object;
}  // namespace mirror
class ArtField;
class ArtMethod;
union JValue;
class ScopedObjectAccessAlreadyRunnable;
class ShadowFrame;

ObjPtr<mirror::Object> BoxPrimitive(Primitive::Type src_class, const JValue& value)
    REQUIRES_SHARED(Locks::mutator_lock_);

bool UnboxPrimitiveForField(ObjPtr<mirror::Object> o,
                            ObjPtr<mirror::Class> dst_class,
                            ArtField* f,
                            JValue* unboxed_value)
    REQUIRES_SHARED(Locks::mutator_lock_);

bool UnboxPrimitiveForResult(ObjPtr<mirror::Object> o,
                             ObjPtr<mirror::Class> dst_class,
                             JValue* unboxed_value)
    REQUIRES_SHARED(Locks::mutator_lock_);

ALWAYS_INLINE bool ConvertPrimitiveValueNoThrow(Primitive::Type src_class,
                                                Primitive::Type dst_class,
                                                const JValue& src,
                                                JValue* dst)
    REQUIRES_SHARED(Locks::mutator_lock_);

ALWAYS_INLINE bool ConvertPrimitiveValue(bool unbox_for_result,
                                         Primitive::Type src_class,
                                         Primitive::Type dst_class,
                                         const JValue& src,
                                         JValue* dst)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Invokes the given method (either an ArtMethod or a jmethodID) with direct/static semantics.
template<typename MethodType>
JValue InvokeWithVarArgs(const ScopedObjectAccessAlreadyRunnable& soa,
                         jobject obj,
                         MethodType mid,
                         va_list args)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Invokes the given method (either an ArtMethod or a jmethodID) with reflection semantics.
template<typename MethodType>
JValue InvokeWithJValues(const ScopedObjectAccessAlreadyRunnable& soa,
                         jobject obj,
                         MethodType mid,
                         const jvalue* args)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Invokes the given method (either an ArtMethod or a jmethodID) with virtual/interface semantics.
// Note this will perform lookup based on the 'obj' to determine which implementation of the given
// method should be invoked.
template<typename MethodType>
JValue InvokeVirtualOrInterfaceWithJValues(const ScopedObjectAccessAlreadyRunnable& soa,
                                           jobject obj,
                                           MethodType mid,
                                           const jvalue* args)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Invokes the given method (either an ArtMethod or a jmethodID) with virtual/interface semantics.
// Note this will perform lookup based on the 'obj' to determine which implementation of the given
// method should be invoked.
template<typename MethodType>
JValue InvokeVirtualOrInterfaceWithVarArgs(const ScopedObjectAccessAlreadyRunnable& soa,
                                           jobject obj,
                                           MethodType mid,
                                           va_list args)
    REQUIRES_SHARED(Locks::mutator_lock_);

// num_frames is number of frames we look up for access check.
template<PointerSize pointer_size>
jobject InvokeMethod(const ScopedObjectAccessAlreadyRunnable& soa,
                     jobject method,
                     jobject receiver,
                     jobject args,
                     size_t num_frames = 1)
    REQUIRES_SHARED(Locks::mutator_lock_);

void ShowVarArgs(const ScopedObjectAccessAlreadyRunnable& ,
                 const char* funcname,
                 const char* data )
    REQUIRES_SHARED(Locks::mutator_lock_);

void ShowVarArgs(const ScopedObjectAccessAlreadyRunnable& ,
                 const char* funcname,
                 jboolean* is_copy ,
                 const char* data )
    REQUIRES_SHARED(Locks::mutator_lock_);

void ShowVarArgs(const ScopedObjectAccessAlreadyRunnable& soa,const char* funcname,
                 jclass java_class, const char* name, const char* sig,jmethodID methodID)
    REQUIRES_SHARED(Locks::mutator_lock_);

void ShowVarArgs(const ScopedObjectAccessAlreadyRunnable& soa,
                 const char* funcname,
                 jmethodID mid,
                 va_list valist)
    REQUIRES_SHARED(Locks::mutator_lock_);

void ShowVarArgs(const ScopedObjectAccessAlreadyRunnable& soa,
                 const char* funcname,
                 jmethodID mid,
                 va_list valist,
                 jobject ret)
REQUIRES_SHARED(Locks::mutator_lock_);



// Special-casing of the above. Assumes that the method is the correct constructor, the class is
// initialized, and that the receiver is an instance of the class.
void InvokeConstructor(const ScopedObjectAccessAlreadyRunnable& soa,
                       ArtMethod* constructor,
                       ObjPtr<mirror::Object> receiver,
                       jobject args)
    REQUIRES_SHARED(Locks::mutator_lock_);

ALWAYS_INLINE bool VerifyObjectIsClass(ObjPtr<mirror::Object> o, ObjPtr<mirror::Class> c)
    REQUIRES_SHARED(Locks::mutator_lock_);

bool VerifyAccess(Thread* self,
                  ObjPtr<mirror::Object> obj,
                  ObjPtr<mirror::Class> declaring_class,
                  uint32_t access_flags,
                  ObjPtr<mirror::Class>* calling_class,
                  size_t num_frames)
    REQUIRES_SHARED(Locks::mutator_lock_);

// This version takes a known calling class.
bool VerifyAccess(ObjPtr<mirror::Object> obj,
                  ObjPtr<mirror::Class> declaring_class,
                  uint32_t access_flags,
                  ObjPtr<mirror::Class> calling_class)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Get the calling class by using a stack visitor, may return null for unattached native threads.
ObjPtr<mirror::Class> GetCallingClass(Thread* self, size_t num_frames)
    REQUIRES_SHARED(Locks::mutator_lock_);

void InvalidReceiverError(ObjPtr<mirror::Object> o, ObjPtr<mirror::Class> c)
    REQUIRES_SHARED(Locks::mutator_lock_);

void UpdateReference(Thread* self, jobject obj, ObjPtr<mirror::Object> result)
    REQUIRES_SHARED(Locks::mutator_lock_);

}  // namespace art

#endif  // ART_RUNTIME_REFLECTION_H_
