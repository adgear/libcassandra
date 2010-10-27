/*
 * LibCassandra
 * Copyright (C) 2010 Padraig O'Sullivan
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license. See
 * the COPYING file in the parent directory for full text.
 */

#ifndef __LIBCASSANDRA_CASSANDRA_FACTORY_H
#define __LIBCASSANDRA_CASSANDRA_FACTORY_H

#include <string>
#include <vector>
#include <tr1/memory>

namespace org 
{ 
namespace apache 
{ 
namespace cassandra 
{

class CassandraClient;

} 
}
}

namespace libcassandra
{

class Cassandra;

class CassandraFactory
{

public:

  CassandraFactory(const std::string &server_list);
  CassandraFactory(const std::string &in_host, int in_port);
  CassandraFactory(const std::string &in_host, int in_port, int in_timeout);
  ~CassandraFactory();

  /**
   * @return a shared ptr which points to a Cassandra client
   */
  std::tr1::shared_ptr<Cassandra> create();
  std::tr1::shared_ptr<Cassandra> create(int framed_transport);

private:

  org::apache::cassandra::CassandraClient *createThriftClient(const std::string &host,
                                                              int port,
							      int conn_timeout,
							      int recv_timeout,
							      int send_timeout
							      );
  org::apache::cassandra::CassandraClient *createThriftClient(const std::string &host,
                                                              int port, 
							      int conn_timeout,
							      int recv_timeout,
							      int send_timeout,
                                                              int framed_transport);

  std::string host;

  int port;
  int conn_timeout;
  int recv_timeout;
  int send_timeout;

};

} /* end namespace libcassandra */

#endif /* __LIBCASSANDRA_CASSANDRA_FACTORY_H */
