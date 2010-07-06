/*
 * LibCassandra
 * Copyright (C) 2010 Padraig O'Sullivan
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license. See
 * the COPYING file in the parent directory for full text.
 */

#include <string>
#include <set>
#include <sstream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <libgenthrift/Cassandra.h>

#include "cassandra.h"
#include "cassandra_factory.h"

using namespace libcassandra;
using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace org::apache::cassandra;
using namespace boost;


CassandraFactory::CassandraFactory(const string &server_list)
  :
    host(),
    port(0)
{
  /* get the host name from the server list string */
  string::size_type pos= server_list.find_first_of(':');
  host= server_list.substr(0, pos);
  /* get the port from the server list string */
  string tmp_port= server_list.substr(pos + 1);
  /* convert to integer */
  istringstream int_stream(tmp_port);
  int_stream >> port;
}

CassandraFactory::CassandraFactory(const string &in_host, int in_port)
  :
    host(in_host),
    port(in_port)
{}

CassandraFactory::~CassandraFactory() {}


tr1::shared_ptr<Cassandra> CassandraFactory::create()
{
  /* Default is Buffered Transport */
  return create(0);
}

tr1::shared_ptr<Cassandra> CassandraFactory::create(int framed_transport)
{
  CassandraClient *thrift_client= createThriftClient(host, port, framed_transport);
  tr1::shared_ptr<Cassandra> ret(new Cassandra(thrift_client, host, port));
  return ret;
}

CassandraClient *CassandraFactory::createThriftClient(const string &in_host,
                                                      int in_port, 
                                                      int framed_transport)
{
  boost::shared_ptr<TTransport> socket(new TSocket(in_host, in_port));
  boost::shared_ptr<TTransport> transport;
  if (framed_transport) 
  {
    transport= boost::shared_ptr<TTransport> (new TFramedTransport(socket));
  }
  else
  {
    transport= boost::shared_ptr<TTransport> (new TBufferedTransport(socket));
  }
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  CassandraClient *client= new(std::nothrow) CassandraClient(protocol);

  transport->open(); /* throws an exception */

  return client;
}

