
/*
 * $Id: client_side_request.h,v 1.12 2003/06/20 12:36:35 robertc Exp $
 *
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#ifndef SQUID_CLIENTSIDEREQUEST_H
#define SQUID_CLIENTSIDEREQUEST_H

#include "HttpHeader.h"
#include "clientStream.h"

/* client_side_request.c - client side request related routines (pure logic) */
extern int clientBeginRequest(method_t, char const *, CSCB *, CSD *, ClientStreamData, HttpHeader const *, char *, size_t);

class MemObject;

typedef class ClientHttpRequest clientHttpRequest;

class ClientHttpRequest
{

public:
    void *operator new (size_t);
    void operator delete (void *);
    void deleteSelf() const;

    ClientHttpRequest();
    ~ClientHttpRequest();
    /* Not implemented - present to prevent synthetic operations */
    ClientHttpRequest(ClientHttpRequest const &);
    ClientHttpRequest& operator=(ClientHttpRequest const &);

    String rangeBoundaryStr() const;
    void freeResources();
    void updateCounters();
    void logRequest();
    _SQUID_INLINE_ MemObject * memObject() const;
    bool multipartRangeRequest() const;
    void processRequest();
    void httpStart();
    bool onlyIfCached()const;
    bool gotEnough() const;
    _SQUID_INLINE_ StoreEntry *storeEntry() const;
    void storeEntry(StoreEntry *);

    ConnStateData *conn;
    request_t *request;		/* Parsed URL ... */
    char *uri;
    char *log_uri;

    struct
    {
        off_t offset;
        size_t size;
        size_t headers_sz;
    }

    out;
    HttpHdrRangeIter range_iter;	/* data for iterating thru range specs */
    size_t req_sz;		/* raw request size on input, not current request size */
    log_type logType;

    struct timeval start;
    http_version_t http_ver;
    AccessLogEntry al;

    struct
    {

unsigned int accel:
        1;

unsigned int transparent:
        1;

unsigned int internal:
        1;

unsigned int done_copying:
        1;

unsigned int purging:
        1;
    }

    flags;

    struct
    {
        http_status status;
        char *location;
    }

    redirect;
    dlink_node active;
    dlink_list client_stream;
    int mRangeCLen();

    ssize_t maxReplyBodySize() const;
    void maxReplyBodySize(ssize_t size);
    bool isReplyBodyTooLarge(ssize_t len) const;

private:
    CBDATA_CLASS(ClientHttpRequest);
    ssize_t maxReplyBodySize_;
    StoreEntry *entry_;
};

/* client http based routines */
SQUIDCEXTERN char *clientConstructTraceEcho(clientHttpRequest *);
SQUIDCEXTERN ACLChecklist *clientAclChecklistCreate(const acl_access * acl, const clientHttpRequest * http);
SQUIDCEXTERN int clientHttpRequestStatus(int fd, clientHttpRequest const *http);
SQUIDCEXTERN void clientAccessCheck(ClientHttpRequest *);

/* ones that should be elsewhere */
SQUIDCEXTERN void redirectStart(clientHttpRequest *, RH *, void *);

SQUIDCEXTERN void sslStart(clientHttpRequest *, size_t *, int *);

#ifdef _USE_INLINE_
#include "Store.h"
#include "client_side_request.cci"
#endif

#endif /* SQUID_CLIENTSIDEREQUEST_H */
