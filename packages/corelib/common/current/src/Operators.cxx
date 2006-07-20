/***************************************************************************
  tag: Peter Soetens  Mon May 10 19:10:37 CEST 2004  Operators.cxx

                        Operators.cxx -  description
                           -------------------
    begin                : Mon May 10 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

#include <rtt/Operators.hpp>
#include <rtt/OperatorTypes.hpp>
#include <rtt/DataSourceAdaptor.hpp>

#include <functional>

// Cappellini Consonni Extension
#include <rtt/MultiVector.hpp>

#include <rtt/Logger.hpp>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>

#include <rtt/mystd.hpp>

#include <pkgconf/corelib.h>
#ifdef OROCFG_CORELIB_GEOMETRY_TOOLKIT_IMPORT
#include <rtt/RealTimeToolkit.hpp>
#endif

namespace RTT
{
    using namespace detail;
    
    namespace {
        boost::shared_ptr<OperatorRepository> reg;
    }

    boost::shared_ptr<OperatorRepository> OperatorRepository::Instance()
    {
        if ( reg )
            return reg;
        reg.reset( new OperatorRepository() );

#ifdef OROCFG_CORELIB_GEOMETRY_TOOLKIT_IMPORT
        Toolkit::Import( RealTimeToolkit );
#endif

        return reg;
    }



  OperatorRepository::OperatorRepository()
  {
  }

  void OperatorRepository::add( DotOp* a )
  {
    dotops.push_back( a );
  }

  void OperatorRepository::add( UnaryOp* a )
  {
    unaryops.push_back( a );
  }

  void OperatorRepository::add( BinaryOp* b )
  {
    binaryops.push_back( b );
  }

  void OperatorRepository::add( TernaryOp* b )
  {
    ternaryops.push_back( b );
  }

  void OperatorRepository::add( SixaryOp* b )
  {
    sixaryops.push_back( b );
  }

  OperatorRepository::~OperatorRepository()
  {
    delete_all( unaryops.begin(), unaryops.end() );
    delete_all( dotops.begin(), dotops.end() );
    delete_all( binaryops.begin(), binaryops.end() );
    delete_all( ternaryops.begin(), ternaryops.end() );
    delete_all( sixaryops.begin(), sixaryops.end() ); 
 }

  DataSourceBase* OperatorRepository::applyDot(
    const std::string& mem, DataSourceBase* a )
  {
    typedef std::vector<DotOp*> vec;
    typedef vec::iterator iter;
    for ( iter i = dotops.begin(); i != dotops.end(); ++i )
    {
      DataSourceBase* ret = (*i)->build( mem, a );
      if ( ret ) return ret;
    }
    return 0;
  }

  DataSourceBase* OperatorRepository::applyUnary(
    const std::string& op, DataSourceBase* a )
  {
    typedef std::vector<UnaryOp*> vec;
    typedef vec::iterator iter;
    for ( iter i = unaryops.begin(); i != unaryops.end(); ++i )
    {
      DataSourceBase* ret = (*i)->build( op, a );
      if ( ret ) return ret;
    }
    return 0;
  }

  DataSourceBase* OperatorRepository::applyBinary(
    const std::string& op, DataSourceBase* a, DataSourceBase* b )
  {
    typedef std::vector<BinaryOp*> vec;
    typedef vec::iterator iter;
    for ( iter i = binaryops.begin(); i != binaryops.end(); ++i )
    {
      DataSourceBase* ret = (*i)->build( op, a, b );
      if ( ret ) return ret;
    }
    return 0;
  }

  DataSourceBase* OperatorRepository::applyTernary(
    const std::string& op, DataSourceBase* a, DataSourceBase* b,
    DataSourceBase* c )
  {
    typedef std::vector<TernaryOp*> vec;
    typedef vec::iterator iter;
    for ( iter i = ternaryops.begin(); i != ternaryops.end(); ++i )
    {
      DataSourceBase* ret = (*i)->build( op, a, b, c );
      if ( ret ) return ret;
    }
    return 0;
  }

  DataSourceBase* OperatorRepository::applySixary(
    const std::string& op,
    DataSourceBase* a, DataSourceBase* b,
    DataSourceBase* c, DataSourceBase* d,
    DataSourceBase* e, DataSourceBase* f )
  {
    typedef std::vector<SixaryOp*> vec;
    typedef vec::iterator iter;
    for ( iter i = sixaryops.begin(); i != sixaryops.end(); ++i )
    {
      DataSourceBase* ret = (*i)->build( op, a, b, c, d, e, f );
      if ( ret ) return ret;
    }
    return 0;
  }

  DotOp::~DotOp()
  {
  }

  UnaryOp::~UnaryOp()
  {
  }

  BinaryOp::~BinaryOp()
  {
  }

  TernaryOp::~TernaryOp()
  {
  }

  SixaryOp::~SixaryOp()
  {
  }
}
