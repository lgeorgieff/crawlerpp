// ============================================================================
// Author: Lukas Georgieff
// File: uri_odb_pragma.h
// Description: This (header) file defines all ncessary ODB pragmas for
//              persisting the uri classes in the namespace crawler_cpp::data.
//              The defined pragmas in this file are excluded from the origin
//              class definition (uri.h) to offer a pure c++ and clean solution
//              since the odb pragmas are unknown to common c++ compilers.
// ============================================================================

#ifndef URI_ODB_PRAGMA
#define URI_ODB_PRAGMA

// required for std::shared_ptr
#include <memory>

// polymorphic:
//  see: http://www.codesynthesis.com/products/odb/doc/manual.xhtml#8.2
// shared_ptr:
//  see: http://www.codesynthesis.com/products/odb/doc/manual.xhtml#3.3
// session:
//  see: http://www.codesynthesis.com/products/odb/doc/manual.xhtml#11
#pragma db object(crawler_pp::data::uri) polymorphic pointer(std::shared_ptr) session(false)
#pragma db member(crawler_pp::data::uri::value_) id type("VARCHAR(2048)")

#pragma db object(crawler_pp::data::waiting_uri)

#pragma db object(crawler_pp::data::visited_uri)

#endif // endif URI_ODB_PRAGMA
