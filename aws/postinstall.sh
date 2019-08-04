#!/bin/bash

. /opt/cfncluster/cfnconfig

if [ "$cfn_node_type" == "MasterServer" ]; then
    # Do something on the MasterServer

    # create a symbolic link to /shared/ec2-user/software
    ln -s /shared/software ~ec2-user/software

    # create SGE queue and pe tailored for our workload
    #export SGE_ROOT=/opt/sge
    #$SGE_ROOT/bin/lx-amd64/qconf -sq all.q | sed "s/all.q/rr.q/" > rr.q
    #$SGE_ROOT/bin/lx-amd64/qconf -sp mpi | sed "s/mpi/mpi_rr/" > mpi_rr.pe
    #$SGE_ROOT/bin/lx-amd64/qconf -Aq rr.q
    #$SGE_ROOT/bin/lx-amd64/qconf -Ap mpi_rr.pe

    #$SGE_ROOT/bin/lx-amd64/qconf -rattr queue pe_list mpi_rr rr.q
    #$SGE_ROOT/bin/lx-amd64/qconf -mattr queue load_thresholds np_load_avg=50 rr.q
    #$SGE_ROOT/bin/lx-amd64/qconf -mattr queue slots 9999 rr.q
    ####for node in `$SGE_ROOT/bin/lx-amd64/qconf -sel`; do $SGE_ROOT/bin/lx-amd64/qconf -purge slots rr.q@$node; done

    #$SGE_ROOT/bin/lx-amd64/qconf -rattr pe allocation_rule \$round_robin mpi_rr
    #rm rr.q mpi_rr.pe
fi

#if [ "$cfn_node_type" == "ComputeFleet" ]; then
#    # Do something on a Compute instance
#fi

# install some software
yum -y update
yum -y groupinstall "Development Tools"
yum -y install gcc72 gcc72-c++ python3 gmp-devel hg zlib-devel boost-devel htop
#yum -y install docker
#yum -y install mpich-devel.x86_64
#yum -y install gmp-devel # needs glibc 2.14 which is not installed in OS

# create profile for setting MPI bin and lib path
cat <<EOF >/etc/profile.d/mpich.sh
#!/bin/bash
export PATH=/usr/lib64/openmpi/bin:\$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:/shared/ec2-user/software/lib:\$LD_LIBRARY_PATH
EOF

# create and populate MPI hostfile in ec2-user root dir
#rm -f ~ec2-user/hostfile.mpi
#for n in `qconf -sel`; do
#    echo ${n/.ec2.internal/} >> ~ec2-user/hostfile.mpi
#done

