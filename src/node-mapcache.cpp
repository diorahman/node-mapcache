/******************************************************************************
 * Copyright (c) 2012, GeoData Institute (www.geodata.soton.ac.uk)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

/**
 * @file node-mapcache.cpp
 * @brief This registers and initialises the module with Node.
 *
 * @mainpage Node %MapCache
 *
 * This represents the C++ bindings that are part of a package
 * providing access to <a
 * href="http://www.mapserver.org/trunk/mapcache/index.html">Mapserver
 * MapCache</a> functionality from <a
 * href="http://nodejs.org">Node.js</a>.
 *
 * MapCache is the primary C++ class which wraps the underlying
 * libmapcache functionality.  A thin layer of javascript is used in
 * the package to expose this class to clients along with some
 * additional features built on it.
 *
 * See the `README.md` file distributed with the package for further
 * details.
 */

#include "mapcache.hpp"

/** Clean up at module exit.
 *
 * This performs housekeeping duties when the module is
 * unloaded. Specifically it frees up static data structures used by
 * the `MapCache` class.
 *
 * The function signature is suitable for using passing it to the
 * `Node::AtExit` function.
 *
 * @param arg Not currently used.
 */
static void Cleanup(void* arg) {
  MapCache::Destroy();
}

/** Initialise the module.
 *
 * This is the entry point to the module called by Node and as such it
 * initialises various module elements:
 *
 * - The APR library used by `libmapcache`.
 * - The `MapCache` class
 * - Versioning information
 *
 * @param target The object representing the module.
 */
extern "C" {
  static void init (Handle<Object> target) {
#ifdef DEBUG
    cout << "Initialising MapCache module" << endl;
#endif
    apr_initialize();
    atexit(apr_terminate);
    apr_pool_initialize();

    MapCache::Init(target);

    // versioning information
    Local<Object> versions = Object::New();
    versions->Set(String::NewSymbol("node_mapcache"), String::New(NODE_MAPCACHE_VERSION));
    versions->Set(String::NewSymbol("mapcache"), String::New(MAPCACHE_VERSION));
    versions->Set(String::NewSymbol("apr"), String::New(APR_VERSION_STRING));
    target->Set(String::NewSymbol("versions"), versions);

    AtExit(Cleanup);
  }
}

/// Register the module
NODE_MODULE(bindings, init)
