INCLUDEPATH += bullet/
DEFINES += USE_MINICL

SOURCES += \
#	bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btDbvt.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
#	bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
#	bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
#	bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
#	bullet/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
#	bullet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
#	bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
#	bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btGhostObject.cpp \
#	bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
#	bullet/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
#	bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
#	bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
#	bullet/BulletCollision/CollisionDispatch/btUnionFind.cpp \
#	bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
#	bullet/BulletCollision/CollisionShapes/btBox2dShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btBoxShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btCollisionShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btCompoundShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConcaveShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConeShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvexShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btCylinderShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btEmptyShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
#	bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btShapeHull.cpp \
#	bullet/BulletCollision/CollisionShapes/btSphereShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
#	bullet/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
#	bullet/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
#	bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
#	bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
#	bullet/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
#	bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
#	bullet/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
#	bullet/BulletCollision/Gimpact/btContactProcessing.cpp \
#	bullet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
#	bullet/BulletCollision/Gimpact/btGImpactBvh.cpp \
#	bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
#	bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
#	bullet/BulletCollision/Gimpact/btGImpactShape.cpp \
#	bullet/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
#	bullet/BulletCollision/Gimpact/gim_box_set.cpp \
#	bullet/BulletCollision/Gimpact/gim_contact.cpp \
#	bullet/BulletCollision/Gimpact/gim_memory.cpp \
#	bullet/BulletCollision/Gimpact/gim_tri_collision.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
#	bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
#	bullet/BulletDynamics/Character/btKinematicCharacterController.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
#	bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
#	bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
#	bullet/BulletDynamics/Dynamics/btRigidBody.cpp \
#	bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
#	bullet/BulletDynamics/Dynamics/Bullet-C-API.cpp \
#	bullet/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
#	bullet/BulletDynamics/Vehicle/btWheelInfo.cpp \
#	bullet/BulletMultiThreaded/btGpu3DGridBroadphase.cpp \
#	bullet/BulletMultiThreaded/btParallelConstraintSolver.cpp \
#	bullet/BulletMultiThreaded/btThreadSupportInterface.cpp \
#	bullet/BulletMultiThreaded/GpuSoftBodySolvers/DX11/btSoftBodySolver_DX11.cpp \
#	bullet/BulletMultiThreaded/GpuSoftBodySolvers/DX11/btSoftBodySolver_DX11SIMDAware.cpp \
#	bullet/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/btSoftBodySolverOutputCLtoGL.cpp \
#	bullet/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/btSoftBodySolver_OpenCL.cpp \
#	bullet/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/btSoftBodySolver_OpenCLSIMDAware.cpp \
#	bullet/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/MiniCL/MiniCLTaskWrap.cpp \
#	bullet/BulletMultiThreaded/PosixThreadSupport.cpp \
#	bullet/BulletMultiThreaded/SequentialThreadSupport.cpp \
#	bullet/BulletMultiThreaded/SpuCollisionObjectWrapper.cpp \
#	bullet/BulletMultiThreaded/SpuCollisionTaskProcess.cpp \
#	bullet/BulletMultiThreaded/SpuContactManifoldCollisionAlgorithm.cpp \
#	bullet/BulletMultiThreaded/SpuFakeDma.cpp \
#	bullet/BulletMultiThreaded/SpuGatheringCollisionDispatcher.cpp \
#	bullet/BulletMultiThreaded/SpuLibspe2Support.cpp \
#	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/boxBoxDistance.cpp \
#	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuCollisionShapes.cpp \
#	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuContactResult.cpp \
#	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.cpp \
#	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.cpp \
#	bullet/BulletMultiThreaded/SpuSampleTask/SpuSampleTask.cpp \
#	bullet/BulletMultiThreaded/SpuSampleTaskProcess.cpp \
#	bullet/BulletMultiThreaded/Win32ThreadSupport.cpp \
	bullet/BulletSoftBody/btDefaultSoftBodySolver.cpp \
	bullet/BulletSoftBody/btSoftBody.cpp \
	bullet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
	bullet/BulletSoftBody/btSoftBodyHelpers.cpp \
	bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
	bullet/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
	bullet/BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
	bullet/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
	bullet/LinearMath/btAlignedAllocator.cpp \
	bullet/LinearMath/btConvexHull.cpp \
	bullet/LinearMath/btConvexHullComputer.cpp \
	bullet/LinearMath/btGeometryUtil.cpp \
	bullet/LinearMath/btPolarDecomposition.cpp \
	bullet/LinearMath/btQuickprof.cpp \
	bullet/LinearMath/btSerializer.cpp \
	bullet/LinearMath/btVector3.cpp \
#	bullet/MiniCL/MiniCL.cpp \
#	bullet/MiniCL/MiniCLTask/MiniCLTask.cpp \
#	bullet/MiniCL/MiniCLTaskScheduler.cpp \


HEADERS += \
	bullet/Bullet-C-Api.h \
	bullet/btBulletDynamicsCommon.h \
	bullet/btBulletCollisionCommon.h \
	bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h \
	bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.h \
	bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h \
	bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h \
	bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h \
	bullet/BulletCollision/BroadphaseCollision/btDispatcher.h \
	bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h \
	bullet/BulletCollision/BroadphaseCollision/btDbvt.h \
	bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h \
	bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h \
	bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h \
	bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h \
	bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.h \
	bullet/BulletCollision/CollisionDispatch/btUnionFind.h \
	bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h \
	bullet/BulletCollision/CollisionDispatch/btManifoldResult.h \
	bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h \
	bullet/BulletCollision/CollisionDispatch/btGhostObject.h \
	bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h \
	bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h \
	bullet/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h \
	bullet/BulletCollision/CollisionDispatch/btCollisionObject.h \
	bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h \
	bullet/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h \
	bullet/BulletCollision/CollisionDispatch/btCollisionConfiguration.h \
	bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.h \
	bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h \
	bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h \
	bullet/BulletCollision/CollisionShapes/btTriangleShape.h \
	bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h \
	bullet/BulletCollision/CollisionShapes/btTriangleMesh.h \
	bullet/BulletCollision/CollisionShapes/btTriangleInfoMap.h \
	bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h \
	bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h \
	bullet/BulletCollision/CollisionShapes/btTriangleCallback.h \
	bullet/BulletCollision/CollisionShapes/btTriangleBuffer.h \
	bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h \
	bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.h \
	bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h \
	bullet/BulletCollision/CollisionShapes/btSphereShape.h \
	bullet/BulletCollision/CollisionShapes/btShapeHull.h \
	bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h \
	bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h \
	bullet/BulletCollision/CollisionShapes/btOptimizedBvh.h \
	bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h \
	bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h \
	bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.h \
	bullet/BulletCollision/CollisionShapes/btMaterial.h \
	bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h \
	bullet/BulletCollision/CollisionShapes/btEmptyShape.h \
	bullet/BulletCollision/CollisionShapes/btCylinderShape.h \
	bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h \
	bullet/BulletCollision/CollisionShapes/btConvexShape.h \
	bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.h \
	bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.h \
	bullet/BulletCollision/CollisionShapes/btConvexInternalShape.h \
	bullet/BulletCollision/CollisionShapes/btConvexHullShape.h \
	bullet/BulletCollision/CollisionShapes/btConvex2dShape.h \
	bullet/BulletCollision/CollisionShapes/btConeShape.h \
	bullet/BulletCollision/CollisionShapes/btConcaveShape.h \
	bullet/BulletCollision/CollisionShapes/btCompoundShape.h \
	bullet/BulletCollision/CollisionShapes/btCollisionShape.h \
	bullet/BulletCollision/CollisionShapes/btCollisionMargin.h \
	bullet/BulletCollision/CollisionShapes/btCapsuleShape.h \
	bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h \
	bullet/BulletCollision/CollisionShapes/btBoxShape.h \
	bullet/BulletCollision/CollisionShapes/btBox2dShape.h \
	bullet/BulletCollision/Gimpact/gim_tri_collision.h \
	bullet/BulletCollision/Gimpact/gim_radixsort.h \
	bullet/BulletCollision/Gimpact/gim_memory.h \
	bullet/BulletCollision/Gimpact/gim_math.h \
	bullet/BulletCollision/Gimpact/gim_linear_math.h \
	bullet/BulletCollision/Gimpact/gim_hash_table.h \
	bullet/BulletCollision/Gimpact/gim_geometry.h \
	bullet/BulletCollision/Gimpact/gim_geom_types.h \
	bullet/BulletCollision/Gimpact/gim_contact.h \
	bullet/BulletCollision/Gimpact/gim_clip_polygon.h \
	bullet/BulletCollision/Gimpact/gim_box_set.h \
	bullet/BulletCollision/Gimpact/gim_box_collision.h \
	bullet/BulletCollision/Gimpact/gim_bitset.h \
	bullet/BulletCollision/Gimpact/gim_basic_geometry_operations.h \
	bullet/BulletCollision/Gimpact/gim_array.h \
	bullet/BulletCollision/Gimpact/btTriangleShapeEx.h \
	bullet/BulletCollision/Gimpact/btQuantization.h \
	bullet/BulletCollision/Gimpact/btGImpactShape.h \
	bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.h \
	bullet/BulletCollision/Gimpact/btGImpactMassUtil.h \
	bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h \
	bullet/BulletCollision/Gimpact/btGImpactBvh.h \
	bullet/BulletCollision/Gimpact/btGeometryOperations.h \
	bullet/BulletCollision/Gimpact/btGenericPoolAllocator.h \
	bullet/BulletCollision/Gimpact/btContactProcessing.h \
	bullet/BulletCollision/Gimpact/btClipPolygon.h \
	bullet/BulletCollision/Gimpact/btBoxCollision.h \
	bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h \
	bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h \
	bullet/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h \
	bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h \
	bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h \
	bullet/BulletCollision/NarrowPhaseCollision/btPointCollector.h \
	bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h \
	bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h \
	bullet/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h \
	bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h \
	bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h \
	bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h \
	bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h \
	bullet/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h \
	bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.h \
	bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h \
	bullet/BulletDynamics/Character/btKinematicCharacterController.h \
	bullet/BulletDynamics/Character/btCharacterControllerInterface.h \
	bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btSolverConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btSolverBody.h \
	bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h \
	bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btJacobianEntry.h \
	bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.h \
	bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btGearConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btContactSolverInfo.h \
	bullet/BulletDynamics/ConstraintSolver/btContactConstraint.h \
	bullet/BulletDynamics/ConstraintSolver/btConstraintSolver.h \
	bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h \
	bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h \
	bullet/BulletDynamics/Dynamics/btRigidBody.h \
	bullet/BulletDynamics/Dynamics/btDynamicsWorld.h \
	bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h \
	bullet/BulletDynamics/Dynamics/btActionInterface.h \
	bullet/BulletDynamics/Vehicle/btWheelInfo.h \
	bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h \
	bullet/BulletDynamics/Vehicle/btRaycastVehicle.h \
	bullet/BulletMultiThreaded/Win32ThreadSupport.h \
	bullet/BulletMultiThreaded/vectormath2bullet.h \
	bullet/BulletMultiThreaded/TrbStateVec.h \
	bullet/BulletMultiThreaded/TrbDynBody.h \
	bullet/BulletMultiThreaded/SpuSync.h \
	bullet/BulletMultiThreaded/SpuSampleTaskProcess.h \
	bullet/BulletMultiThreaded/SpuLibspe2Support.h \
	bullet/BulletMultiThreaded/SpuGatheringCollisionDispatcher.h \
	bullet/BulletMultiThreaded/SpuFakeDma.h \
	bullet/BulletMultiThreaded/SpuDoubleBuffer.h \
	bullet/BulletMultiThreaded/SpuContactManifoldCollisionAlgorithm.h \
	bullet/BulletMultiThreaded/SpuCollisionTaskProcess.h \
	bullet/BulletMultiThreaded/SpuCollisionObjectWrapper.h \
	bullet/BulletMultiThreaded/SequentialThreadSupport.h \
	bullet/BulletMultiThreaded/PpuAddressSpace.h \
	bullet/BulletMultiThreaded/PosixThreadSupport.h \
	bullet/BulletMultiThreaded/PlatformDefinitions.h \
	bullet/BulletMultiThreaded/HeapManager.h \
	bullet/BulletMultiThreaded/btThreadSupportInterface.h \
	bullet/BulletMultiThreaded/btParallelConstraintSolver.h \
	bullet/BulletMultiThreaded/btGpuUtilsSharedDefs.h \
	bullet/BulletMultiThreaded/btGpuUtilsSharedCode.h \
	bullet/BulletMultiThreaded/btGpuDefines.h \
	bullet/BulletMultiThreaded/btGpu3DGridBroadphaseSharedTypes.h \
	bullet/BulletMultiThreaded/btGpu3DGridBroadphaseSharedDefs.h \
	bullet/BulletMultiThreaded/btGpu3DGridBroadphaseSharedCode.h \
	bullet/BulletMultiThreaded/btGpu3DGridBroadphase.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuPreferredPenetrationDirections.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuLocalSupport.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuConvexPenetrationDepthSolver.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuContactResult.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuCollisionShapes.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/boxBoxDistance.h \
	bullet/BulletMultiThreaded/SpuNarrowPhaseCollisionTask/Box.h \
	bullet/BulletMultiThreaded/SpuSampleTask/SpuSampleTask.h \
	bullet/BulletSoftBody/btSparseSDF.h \
	bullet/BulletSoftBody/btSoftSoftCollisionAlgorithm.h \
	bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h \
	bullet/BulletSoftBody/btSoftRigidCollisionAlgorithm.h \
	bullet/BulletSoftBody/btSoftBodySolverVertexBuffer.h \
	bullet/BulletSoftBody/btSoftBodySolvers.h \
	bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h \
	bullet/BulletSoftBody/btSoftBodyInternals.h \
	bullet/BulletSoftBody/btSoftBodyHelpers.h \
	bullet/BulletSoftBody/btSoftBodyData.h \
	bullet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h \
	bullet/BulletSoftBody/btSoftBody.h \
	bullet/BulletSoftBody/btDefaultSoftBodySolver.h \
	bullet/LinearMath/btVector3.h \
	bullet/LinearMath/btTransformUtil.h \
	bullet/LinearMath/btTransform.h \
	bullet/LinearMath/btStackAlloc.h \
	bullet/LinearMath/btSerializer.h \
	bullet/LinearMath/btScalar.h \
	bullet/LinearMath/btRandom.h \
	bullet/LinearMath/btQuickprof.h \
	bullet/LinearMath/btQuaternion.h \
	bullet/LinearMath/btQuadWord.h \
	bullet/LinearMath/btPoolAllocator.h \
	bullet/LinearMath/btPolarDecomposition.h \
	bullet/LinearMath/btMotionState.h \
	bullet/LinearMath/btMinMax.h \
	bullet/LinearMath/btMatrix3x3.h \
	bullet/LinearMath/btList.h \
	bullet/LinearMath/btIDebugDraw.h \
	bullet/LinearMath/btHashMap.h \
	bullet/LinearMath/btGrahamScan2dConvexHull.h \
	bullet/LinearMath/btGeometryUtil.h \
	bullet/LinearMath/btDefaultMotionState.h \
	bullet/LinearMath/btConvexHullComputer.h \
	bullet/LinearMath/btConvexHull.h \
	bullet/LinearMath/btAlignedObjectArray.h \
	bullet/LinearMath/btAlignedAllocator.h \
	bullet/LinearMath/btAabbUtil2.h