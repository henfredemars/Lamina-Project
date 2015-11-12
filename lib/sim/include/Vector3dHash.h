
//Hash function for storing Eigen::Vector3d in hash tables
//WARNING! Eigen doesn't define this because vectors are mutable
//  If you use this, dont mutate your vectors

#ifndef __VECTOR3DHASH_H
#define __VECTOR3DHASH_H

#include "../../eigen/Dense"

class Vector3dHash
{
public:
    std::size_t operator()(const Eigen::Vector3d &v) const
    {
        std::size_t x = (long)v(0)+v(1)*1000+v(2)*v(2);
        return x*1000;
    }
};


#endif /* __VECTOR3DHASH_H */
