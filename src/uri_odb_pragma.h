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

#pragma db object(crawler_cpp::data::uri)
#pragma db member(crawler_cpp::data::uri::uri_) id

#endif // endif URI_ODB_PRAGMA
