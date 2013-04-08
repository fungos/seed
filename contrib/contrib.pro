QT -= qt

TARGET = seedcontrib
TEMPLATE = lib

#TARGET_EXT = .bc
#QMAKE_EXT_OBJ = .bc
#QMAKE_CXXFLAGS += -emit-llvm
#QMAKE_CXX = clang++
#QMAKE_CC = clang
#QMAKE_LIB = llvm-ld
#QMAKE_RUN_CXX = $(CXX) $(CXXFLAGS) $(INCPATH) -c $src -o $obj
#QMAKE_RUN_CC = $(CC) $(CCFLAGS) $(INCPATH) -c $src -o $obj

QMAKE_CXXFLAGS -= -fno-rtti

CONFIG(debug, debug|release) {
	DESTDIR = ../lib/debug
	DEFINES += DEBUG
} else {
	DESTDIR = ../lib/release
	DEFINES += RELEASE
}

# bullet specifics
INCLUDEPATH += bullet/
DEFINES += USE_MINICL
# bullet


CONFIG += staticlib

SOURCES += yajl/yajl_buf.c \
	yajl/yajl_alloc.c \
	yajl/yajl.c \
	yajl/yajl_version.c \
	yajl/yajl_tree.c \
	yajl/yajl_parser.c \
	yajl/yajl_lex.c \
	yajl/yajl_gen.c \
	yajl/yajl_encode.c \
	vorbis/vorbisfile.c \
	vorbis/vorbisenc.c \
	vorbis/synthesis.c \
	vorbis/smallft.c \
	vorbis/sharedbook.c \
	vorbis/res0.c \
	vorbis/registry.c \
	vorbis/psy.c \
	vorbis/mdct.c \
	vorbis/mapping0.c \
	vorbis/lsp.c \
	vorbis/lpc.c \
	vorbis/lookup.c \
	vorbis/info.c \
	vorbis/floor1.c \
	vorbis/floor0.c \
	vorbis/envelope.c \
	vorbis/codebook.c \
	vorbis/block.c \
	vorbis/bitrate.c \
	vorbis/analysis.c \
	theora/tokenize.c \
	theora/thinfo.c \
	theora/state.c \
	theora/rate.c \
	theora/quant.c \
	theora/mcenc.c \
	theora/mathops.c \
	theora/internal.c \
	theora/idct.c \
	theora/huffenc.c \
	theora/huffdec.c \
	theora/fragment.c \
	theora/fdct.c \
	theora/enquant.c \
	theora/encode.c \
	theora/encinfo.c \
	theora/encfrag.c \
	theora/encapiwrapper.c \
	theora/dequant.c \
	theora/decode.c \
	theora/decinfo.c \
	theora/decapiwrapper.c \
	theora/cpu.c \
	theora/bitpack.c \
	theora/apiwrapper.c \
	theora/analyze.c \
	physfs/physfs_unicode.c \
	physfs/physfs_byteorder.c \
	physfs/physfs.c \
	physfs/zlib123/zutil.c \
	physfs/zlib123/trees.c \
	physfs/zlib123/uncompr.c \
	physfs/zlib123/inftrees.c \
	physfs/zlib123/inflate.c \
	physfs/zlib123/deflate.c \
	physfs/zlib123/inffast.c \
	physfs/zlib123/infback.c \
	physfs/zlib123/gzio.c \
	physfs/zlib123/crc32.c \
	physfs/zlib123/compress.c \
	physfs/zlib123/adler32.c \
	physfs/platform/windows.c \
	physfs/platform/unix.c \
	physfs/platform/posix.c \
	physfs/platform/pocketpc.c \
	physfs/platform/os2.c \
	physfs/platform/macosx.c \
	physfs/platform/beos.cpp \
	physfs/archivers/zip.c \
	physfs/archivers/wad.c \
	physfs/archivers/qpak.c \
	physfs/archivers/mvl.c \
	physfs/archivers/lzma.c \
	physfs/archivers/hog.c \
	physfs/archivers/grp.c \
	physfs/archivers/dir.c \
	oggz/oggz_write.c \
	oggz/oggz_vector.c \
	oggz/oggz_table.c \
	oggz/oggz_stream.c \
	oggz/oggz_seek.c \
	oggz/oggz_read.c \
	oggz/oggz_io.c \
	oggz/oggz_dlist.c \
	oggz/oggz_comments.c \
	oggz/oggz_auto.c \
	oggz/oggz.c \
	oggz/metric_internal.c \
	oggz/dirac.c \
	oggplay/oggplay_yuv2rgb.c \
	oggplay/oggplay_tools.c \
	oggplay/oggplay_tcp_reader.c \
	oggplay/oggplay_seek.c \
	oggplay/oggplay_query.c \
	oggplay/oggplay_file_reader.c \
	oggplay/oggplay_data.c \
	oggplay/oggplay_callback_info.c \
	oggplay/oggplay_callback.c \
	oggplay/oggplay_buffer.c \
	oggplay/oggplay.c \
	oggplay/cpu.c \
	ogg/framing.c \
	ogg/bitwise.c \
	glew/glew.c \
	Box2D/Collision/b2TimeOfImpact.cpp \
	Box2D/Collision/b2DynamicTree.cpp \
	Box2D/Collision/b2Distance.cpp \
	Box2D/Collision/b2Collision.cpp \
	Box2D/Collision/b2CollidePolygon.cpp \
	Box2D/Collision/b2CollideCircle.cpp \
	Box2D/Collision/b2CollideEdge.cpp \
	Box2D/Collision/b2BroadPhase.cpp \
	Box2D/Collision/Shapes/b2ChainShape.cpp \
	Box2D/Collision/Shapes/b2EdgeShape.cpp \
	Box2D/Collision/Shapes/b2PolygonShape.cpp \
	Box2D/Collision/Shapes/b2CircleShape.cpp \
	Box2D/Common/b2StackAllocator.cpp \
	Box2D/Common/b2Draw.cpp \
	Box2D/Common/b2Timer.cpp \
	Box2D/Common/b2Settings.cpp \
	Box2D/Common/b2Math.cpp \
	Box2D/Common/b2BlockAllocator.cpp \
	Box2D/Dynamics/b2WorldCallbacks.cpp \
	Box2D/Dynamics/b2World.cpp \
	Box2D/Dynamics/b2Island.cpp \
	Box2D/Dynamics/b2Fixture.cpp \
	Box2D/Dynamics/b2ContactManager.cpp \
	Box2D/Dynamics/b2Body.cpp \
	Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
	Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
	Box2D/Dynamics/Contacts/b2CircleContact.cpp \
	Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
	Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
	Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
	Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
	Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
	Box2D/Dynamics/Contacts/b2Contact.cpp \
	Box2D/Dynamics/Joints/b2WeldJoint.cpp \
	Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
	Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
	Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
	Box2D/Dynamics/Joints/b2MouseJoint.cpp \
	Box2D/Dynamics/Joints/b2Joint.cpp \
	Box2D/Dynamics/Joints/b2GearJoint.cpp \
	Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
	Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
	Box2D/Dynamics/Joints/b2RopeJoint.cpp \
	Box2D/Dynamics/Joints/b2WheelJoint.cpp \
	Box2D/Rope/b2Rope.cpp \
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
	vectormath/vfpmath/vsinf.cpp \
	vectormath/vfpmath/matrix_impl.cpp \
	lua/lzio.c \
	lua/lvm.c \
	lua/lundump.c \
	lua/ltm.c \
	lua/ltablib.c \
	lua/ltable.c \
	lua/lstrlib.c \
	lua/lstring.c \
	lua/lstate.c \
	lua/lparser.c \
	lua/loslib.c \
	lua/lopcodes.c \
	lua/lobject.c \
	lua/loadlib.c \
	lua/lmem.c \
	lua/lmathlib.c \
	lua/llex.c \
	lua/liolib.c \
	lua/linit.c \
	lua/lgc.c \
	lua/lfunc.c \
	lua/ldump.c \
	lua/ldo.c \
	lua/ldebug.c \
	lua/ldblib.c \
	lua/lctype.c \
	lua/lcorolib.c \
	lua/lcode.c \
	lua/lbitlib.c \
	lua/lbaselib.c \
	lua/lauxlib.c \
	lua/lapi.c \
	glfw/window.c \
	glfw/time.c \
	glfw/thread.c \
	glfw/tga.c \
	glfw/stream.c \
	glfw/joystick.c \
	glfw/input.c \
	glfw/init.c \
	glfw/image.c \
	glfw/glext.c \
	glfw/fullscreen.c \
	glfw/enable.c \
	vorbis/vorbiswindow.c \
	soil/stb_image_aug.c \
	soil/SOIL.c \
	soil/image_helper.c \
	soil/image_DXT.c \
	freetype/src/autofit/afangles.c \
	freetype/src/autofit/afcjk.c \
	freetype/src/autofit/afdummy.c \
	freetype/src/autofit/afglobal.c \
	freetype/src/autofit/afhints.c \
	freetype/src/autofit/afindic.c \
	freetype/src/autofit/aflatin.c \
	freetype/src/autofit/aflatin2.c \
	freetype/src/autofit/afloader.c \
	freetype/src/autofit/afmodule.c \
	freetype/src/autofit/afpic.c \
	freetype/src/autofit/afwarp.c \
	freetype/src/autofit/autofit.c \
	freetype/src/base/basepic.c \
	freetype/src/base/ftadvanc.c \
	freetype/src/base/ftapi.c \
	freetype/src/base/ftbase.c \
	freetype/src/base/ftbbox.c \
	freetype/src/base/ftbdf.c \
	freetype/src/base/ftbitmap.c \
	freetype/src/base/ftcalc.c \
	freetype/src/base/ftcid.c \
	freetype/src/base/ftdbgmem.c \
	freetype/src/base/ftdebug.c \
	freetype/src/base/ftfstype.c \
	freetype/src/base/ftgasp.c \
	freetype/src/base/ftgloadr.c \
	freetype/src/base/ftglyph.c \
	freetype/src/base/ftgxval.c \
	freetype/src/base/ftinit.c \
	freetype/src/base/ftlcdfil.c \
	freetype/src/base/ftmac.c \
	freetype/src/base/ftmm.c \
	freetype/src/base/ftobjs.c \
	freetype/src/base/ftotval.c \
	freetype/src/base/ftoutln.c \
	freetype/src/base/ftpatent.c \
	freetype/src/base/ftpfr.c \
	freetype/src/base/ftpic.c \
	freetype/src/base/ftrfork.c \
	freetype/src/base/ftsnames.c \
	freetype/src/base/ftstream.c \
	freetype/src/base/ftstroke.c \
	freetype/src/base/ftsynth.c \
	freetype/src/base/ftsystem.c \
	freetype/src/base/fttrigon.c \
	freetype/src/base/fttype1.c \
	freetype/src/base/ftutil.c \
	freetype/src/base/ftwinfnt.c \
	freetype/src/base/ftxf86.c \
	freetype/src/bdf/bdf.c \
	freetype/src/bdf/bdfdrivr.c \
	freetype/src/bdf/bdflib.c \
	freetype/src/bzip2/ftbzip2.c \
	freetype/src/cache/ftcache.c \
	freetype/src/cache/ftcbasic.c \
	freetype/src/cache/ftccache.c \
	freetype/src/cache/ftccmap.c \
	freetype/src/cache/ftcglyph.c \
	freetype/src/cache/ftcimage.c \
	freetype/src/cache/ftcmanag.c \
	freetype/src/cache/ftcmru.c \
	freetype/src/cache/ftcsbits.c \
	freetype/src/cff/cff.c \
	freetype/src/cff/cffcmap.c \
	freetype/src/cff/cffdrivr.c \
	freetype/src/cff/cffgload.c \
	freetype/src/cff/cffload.c \
	freetype/src/cff/cffobjs.c \
	freetype/src/cff/cffparse.c \
	freetype/src/cff/cffpic.c \
	freetype/src/cid/cidgload.c \
	freetype/src/cid/cidload.c \
	freetype/src/cid/cidobjs.c \
	freetype/src/cid/cidparse.c \
	freetype/src/cid/cidriver.c \
	freetype/src/cid/type1cid.c \
	freetype/src/gxvalid/gxvalid.c \
	freetype/src/gxvalid/gxvbsln.c \
	freetype/src/gxvalid/gxvcommn.c \
	freetype/src/gxvalid/gxvfeat.c \
	freetype/src/gxvalid/gxvfgen.c \
	freetype/src/gxvalid/gxvjust.c \
	freetype/src/gxvalid/gxvkern.c \
	freetype/src/gxvalid/gxvlcar.c \
	freetype/src/gxvalid/gxvmod.c \
	freetype/src/gxvalid/gxvmort.c \
	freetype/src/gxvalid/gxvmort0.c \
	freetype/src/gxvalid/gxvmort1.c \
	freetype/src/gxvalid/gxvmort2.c \
	freetype/src/gxvalid/gxvmort4.c \
	freetype/src/gxvalid/gxvmort5.c \
	freetype/src/gxvalid/gxvmorx.c \
	freetype/src/gxvalid/gxvmorx0.c \
	freetype/src/gxvalid/gxvmorx1.c \
	freetype/src/gxvalid/gxvmorx2.c \
	freetype/src/gxvalid/gxvmorx4.c \
	freetype/src/gxvalid/gxvmorx5.c \
	freetype/src/gxvalid/gxvopbd.c \
	freetype/src/gxvalid/gxvprop.c \
	freetype/src/gxvalid/gxvtrak.c \
	freetype/src/gzip/adler32.c \
	freetype/src/gzip/ftgzip.c \
	freetype/src/gzip/infblock.c \
	freetype/src/gzip/infcodes.c \
	freetype/src/gzip/inflate.c \
	freetype/src/gzip/inftrees.c \
	freetype/src/gzip/infutil.c \
	freetype/src/gzip/zutil.c \
	freetype/src/lzw/ftlzw.c \
	freetype/src/lzw/ftzopen.c \
	freetype/src/otvalid/otvalid.c \
	freetype/src/otvalid/otvbase.c \
	freetype/src/otvalid/otvcommn.c \
	freetype/src/otvalid/otvgdef.c \
	freetype/src/otvalid/otvgpos.c \
	freetype/src/otvalid/otvgsub.c \
	freetype/src/otvalid/otvjstf.c \
	freetype/src/otvalid/otvmath.c \
	freetype/src/otvalid/otvmod.c \
	freetype/src/pcf/pcf.c \
	freetype/src/pcf/pcfdrivr.c \
	freetype/src/pcf/pcfread.c \
	freetype/src/pcf/pcfutil.c \
	freetype/src/pfr/pfr.c \
	freetype/src/pfr/pfrcmap.c \
	freetype/src/pfr/pfrdrivr.c \
	freetype/src/pfr/pfrgload.c \
	freetype/src/pfr/pfrload.c \
	freetype/src/pfr/pfrobjs.c \
	freetype/src/pfr/pfrsbit.c \
	freetype/src/psaux/afmparse.c \
	freetype/src/psaux/psaux.c \
	freetype/src/psaux/psauxmod.c \
	freetype/src/psaux/psconv.c \
	freetype/src/psaux/psobjs.c \
	freetype/src/psaux/t1cmap.c \
	freetype/src/psaux/t1decode.c \
	freetype/src/pshinter/pshalgo.c \
	freetype/src/pshinter/pshglob.c \
	freetype/src/pshinter/pshinter.c \
	freetype/src/pshinter/pshmod.c \
	freetype/src/pshinter/pshpic.c \
	freetype/src/pshinter/pshrec.c \
	freetype/src/psnames/psmodule.c \
	freetype/src/psnames/psnames.c \
	freetype/src/psnames/pspic.c \
	freetype/src/raster/ftraster.c \
	freetype/src/raster/ftrend1.c \
	freetype/src/raster/raster.c \
	freetype/src/raster/rastpic.c \
	freetype/src/sfnt/sfdriver.c \
	freetype/src/sfnt/sfnt.c \
	freetype/src/sfnt/sfntpic.c \
	freetype/src/sfnt/sfobjs.c \
	freetype/src/sfnt/ttbdf.c \
	freetype/src/sfnt/ttcmap.c \
	freetype/src/sfnt/ttkern.c \
	freetype/src/sfnt/ttload.c \
	freetype/src/sfnt/ttmtx.c \
	freetype/src/sfnt/ttpost.c \
	freetype/src/sfnt/ttsbit.c \
	freetype/src/sfnt/ttsbit0.c \
	freetype/src/smooth/ftgrays.c \
	freetype/src/smooth/ftsmooth.c \
	freetype/src/smooth/ftspic.c \
	freetype/src/smooth/smooth.c \
	freetype/src/truetype/truetype.c \
	freetype/src/truetype/ttdriver.c \
	freetype/src/truetype/ttgload.c \
	freetype/src/truetype/ttgxvar.c \
	freetype/src/truetype/ttinterp.c \
	freetype/src/truetype/ttobjs.c \
	freetype/src/truetype/ttpic.c \
	freetype/src/truetype/ttpload.c \
	freetype/src/type1/t1afm.c \
	freetype/src/type1/t1driver.c \
	freetype/src/type1/t1gload.c \
	freetype/src/type1/t1load.c \
	freetype/src/type1/t1objs.c \
	freetype/src/type1/t1parse.c \
	freetype/src/type1/type1.c \
	freetype/src/type42/t42drivr.c \
	freetype/src/type42/t42objs.c \
	freetype/src/type42/t42parse.c \
	freetype/src/type42/type42.c \
	freetype/src/winfonts/winfnt.c \
	Rocket/Source/Controls/Clipboard.cpp \
	Rocket/Source/Controls/Controls.cpp \
	Rocket/Source/Controls/DataFormatter.cpp \
	Rocket/Source/Controls/DataQuery.cpp \
	Rocket/Source/Controls/DataSource.cpp \
	Rocket/Source/Controls/DataSourceListener.cpp \
	Rocket/Source/Controls/ElementDataGrid.cpp \
	Rocket/Source/Controls/ElementDataGridCell.cpp \
	Rocket/Source/Controls/ElementDataGridExpandButton.cpp \
	Rocket/Source/Controls/ElementDataGridRow.cpp \
	Rocket/Source/Controls/ElementForm.cpp \
	Rocket/Source/Controls/ElementFormControl.cpp \
	Rocket/Source/Controls/ElementFormControlDataSelect.cpp \
	Rocket/Source/Controls/ElementFormControlInput.cpp \
	Rocket/Source/Controls/ElementFormControlSelect.cpp \
	Rocket/Source/Controls/ElementFormControlTextArea.cpp \
	Rocket/Source/Controls/ElementTabSet.cpp \
	Rocket/Source/Controls/ElementTextSelection.cpp \
	Rocket/Source/Controls/InputType.cpp \
	Rocket/Source/Controls/InputTypeButton.cpp \
	Rocket/Source/Controls/InputTypeCheckbox.cpp \
	Rocket/Source/Controls/InputTypeRadio.cpp \
	Rocket/Source/Controls/InputTypeRange.cpp \
	Rocket/Source/Controls/InputTypeSubmit.cpp \
	Rocket/Source/Controls/InputTypeText.cpp \
	Rocket/Source/Controls/SelectOption.cpp \
	Rocket/Source/Controls/WidgetDropDown.cpp \
	Rocket/Source/Controls/WidgetSlider.cpp \
	Rocket/Source/Controls/WidgetSliderInput.cpp \
	Rocket/Source/Controls/WidgetTextInput.cpp \
	Rocket/Source/Controls/WidgetTextInputMultiLine.cpp \
	Rocket/Source/Controls/WidgetTextInputSingleLine.cpp \
	Rocket/Source/Controls/WidgetTextInputSingleLinePassword.cpp \
	Rocket/Source/Controls/XMLNodeHandlerDataGrid.cpp \
	Rocket/Source/Controls/XMLNodeHandlerTabSet.cpp \
	Rocket/Source/Controls/XMLNodeHandlerTextArea.cpp \
	Rocket/Source/Core/BaseXMLParser.cpp \
	Rocket/Source/Core/Box.cpp \
	Rocket/Source/Core/Clock.cpp \
	Rocket/Source/Core/Context.cpp \
	Rocket/Source/Core/ContextInstancer.cpp \
	Rocket/Source/Core/ContextInstancerDefault.cpp \
	Rocket/Source/Core/ConvolutionFilter.cpp \
	Rocket/Source/Core/Core.cpp \
	Rocket/Source/Core/Decorator.cpp \
	Rocket/Source/Core/DecoratorInstancer.cpp \
	Rocket/Source/Core/DecoratorNone.cpp \
	Rocket/Source/Core/DecoratorNoneInstancer.cpp \
	Rocket/Source/Core/DecoratorTiled.cpp \
	Rocket/Source/Core/DecoratorTiledBox.cpp \
	Rocket/Source/Core/DecoratorTiledBoxInstancer.cpp \
	Rocket/Source/Core/DecoratorTiledHorizontal.cpp \
	Rocket/Source/Core/DecoratorTiledHorizontalInstancer.cpp \
	Rocket/Source/Core/DecoratorTiledImage.cpp \
	Rocket/Source/Core/DecoratorTiledImageInstancer.cpp \
	Rocket/Source/Core/DecoratorTiledInstancer.cpp \
	Rocket/Source/Core/DecoratorTiledVertical.cpp \
	Rocket/Source/Core/DecoratorTiledVerticalInstancer.cpp \
	Rocket/Source/Core/Dictionary.cpp \
	Rocket/Source/Core/DocumentHeader.cpp \
	Rocket/Source/Core/Element.cpp \
	Rocket/Source/Core/ElementBackground.cpp \
	Rocket/Source/Core/ElementBorder.cpp \
	Rocket/Source/Core/ElementDecoration.cpp \
	Rocket/Source/Core/ElementDefinition.cpp \
	Rocket/Source/Core/ElementDocument.cpp \
	Rocket/Source/Core/ElementHandle.cpp \
	Rocket/Source/Core/ElementImage.cpp \
	Rocket/Source/Core/ElementInstancer.cpp \
	Rocket/Source/Core/ElementReference.cpp \
	Rocket/Source/Core/ElementScroll.cpp \
	Rocket/Source/Core/ElementStyle.cpp \
	Rocket/Source/Core/ElementText.cpp \
	Rocket/Source/Core/ElementTextDefault.cpp \
	Rocket/Source/Core/ElementUtilities.cpp \
	Rocket/Source/Core/Event.cpp \
	Rocket/Source/Core/EventDispatcher.cpp \
	Rocket/Source/Core/EventInstancer.cpp \
	Rocket/Source/Core/EventInstancerDefault.cpp \
	Rocket/Source/Core/EventListenerInstancer.cpp \
	Rocket/Source/Core/Factory.cpp \
	Rocket/Source/Core/FileInterface.cpp \
	Rocket/Source/Core/FileInterfaceDefault.cpp \
	Rocket/Source/Core/FontDatabase.cpp \
	Rocket/Source/Core/FontEffect.cpp \
	Rocket/Source/Core/FontEffectInstancer.cpp \
	Rocket/Source/Core/FontEffectNone.cpp \
	Rocket/Source/Core/FontEffectNoneInstancer.cpp \
	Rocket/Source/Core/FontEffectOutline.cpp \
	Rocket/Source/Core/FontEffectOutlineInstancer.cpp \
	Rocket/Source/Core/FontEffectShadow.cpp \
	Rocket/Source/Core/FontEffectShadowInstancer.cpp \
	Rocket/Source/Core/FontFace.cpp \
	Rocket/Source/Core/FontFaceHandle.cpp \
	Rocket/Source/Core/FontFaceLayer.cpp \
	Rocket/Source/Core/FontFamily.cpp \
	Rocket/Source/Core/Geometry.cpp \
	Rocket/Source/Core/GeometryDatabase.cpp \
	Rocket/Source/Core/GeometryUtilities.cpp \
	Rocket/Source/Core/LayoutBlockBox.cpp \
	Rocket/Source/Core/LayoutBlockBoxSpace.cpp \
	Rocket/Source/Core/LayoutEngine.cpp \
	Rocket/Source/Core/LayoutInlineBox.cpp \
	Rocket/Source/Core/LayoutInlineBoxText.cpp \
	Rocket/Source/Core/LayoutLineBox.cpp \
	Rocket/Source/Core/Log.cpp \
	Rocket/Source/Core/Math.cpp \
	Rocket/Source/Core/Plugin.cpp \
	Rocket/Source/Core/PluginRegistry.cpp \
	Rocket/Source/Core/precompiled.cpp \
	Rocket/Source/Core/Property.cpp \
	Rocket/Source/Core/PropertyDefinition.cpp \
	Rocket/Source/Core/PropertyDictionary.cpp \
	Rocket/Source/Core/PropertyParserColour.cpp \
	Rocket/Source/Core/PropertyParserKeyword.cpp \
	Rocket/Source/Core/PropertyParserNumber.cpp \
	Rocket/Source/Core/PropertyParserString.cpp \
	Rocket/Source/Core/PropertySpecification.cpp \
	Rocket/Source/Core/ReferenceCountable.cpp \
	Rocket/Source/Core/RenderInterface.cpp \
	Rocket/Source/Core/RocketStream.cpp \
	Rocket/Source/Core/StreamFile.cpp \
	Rocket/Source/Core/StreamMemory.cpp \
	Rocket/Source/Core/String.cpp \
	Rocket/Source/Core/StringCache.cpp \
	Rocket/Source/Core/StringUtilities.cpp \
	Rocket/Source/Core/StyleSheet.cpp \
	Rocket/Source/Core/StyleSheetFactory.cpp \
	Rocket/Source/Core/StyleSheetNode.cpp \
	Rocket/Source/Core/StyleSheetNodeSelector.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorEmpty.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorFirstChild.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorFirstOfType.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorLastChild.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorLastOfType.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorNthChild.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorNthLastChild.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorNthLastOfType.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorNthOfType.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorOnlyChild.cpp \
	Rocket/Source/Core/StyleSheetNodeSelectorOnlyOfType.cpp \
	Rocket/Source/Core/StyleSheetParser.cpp \
	Rocket/Source/Core/StyleSheetSpecification.cpp \
	Rocket/Source/Core/SystemInterface.cpp \
	Rocket/Source/Core/Template.cpp \
	Rocket/Source/Core/TemplateCache.cpp \
	Rocket/Source/Core/Texture.cpp \
	Rocket/Source/Core/TextureDatabase.cpp \
	Rocket/Source/Core/TextureLayout.cpp \
	Rocket/Source/Core/TextureLayoutRectangle.cpp \
	Rocket/Source/Core/TextureLayoutRow.cpp \
	Rocket/Source/Core/TextureLayoutTexture.cpp \
	Rocket/Source/Core/TextureResource.cpp \
	Rocket/Source/Core/UnicodeRange.cpp \
	Rocket/Source/Core/URL.cpp \
	Rocket/Source/Core/Variant.cpp \
	Rocket/Source/Core/Vector2.cpp \
	Rocket/Source/Core/RocketWidgetSlider.cpp \
	Rocket/Source/Core/WidgetSliderScroll.cpp \
	Rocket/Source/Core/WString.cpp \
	Rocket/Source/Core/XMLNodeHandler.cpp \
	Rocket/Source/Core/XMLNodeHandlerBody.cpp \
	Rocket/Source/Core/XMLNodeHandlerDefault.cpp \
	Rocket/Source/Core/XMLNodeHandlerHead.cpp \
	Rocket/Source/Core/XMLNodeHandlerTemplate.cpp \
	Rocket/Source/Core/XMLParser.cpp \
	Rocket/Source/Core/XMLParseTools.cpp \
	Rocket/Source/Debugger/Debugger.cpp \
	Rocket/Source/Debugger/ElementContextHook.cpp \
	Rocket/Source/Debugger/ElementInfo.cpp \
	Rocket/Source/Debugger/ElementLog.cpp \
	Rocket/Source/Debugger/DebugGeometry.cpp \
	Rocket/Source/Debugger/DebugPlugin.cpp \
	Rocket/Source/Debugger/DebugSystemInterface.cpp

unix:!macx:SOURCES += \
	glfw/x11/x11_enable.c \
	glfw/x11/x11_fullscreen.c \
	glfw/x11/x11_glext.c \
	glfw/x11/x11_init.c \
	glfw/x11/x11_joystick.c \
	glfw/x11/x11_keysym2unicode.c \
	glfw/x11/x11_thread.c \
	glfw/x11/x11_time.c \
	glfw/x11/x11_window.c

macx:OBJECTIVE_SOURCES += \
	glfw/cocoa/cocoa_enable.m \
	glfw/cocoa/cocoa_fullscreen.m \
	glfw/cocoa/cocoa_glext.m \
	glfw/cocoa/cocoa_init.m \
	glfw/cocoa/cocoa_joystick.m \
	glfw/cocoa/cocoa_thread.c \
	glfw/cocoa/cocoa_time.m \
	glfw/cocoa/cocoa_window.m

win32:SOURCES += \
	glfw/win32/win32_enable.c \
	glfw/win32/win32_fullscreen.c \
	glfw/win32/win32_glext.c \
	glfw/win32/win32_init.c \
	glfw/win32/win32_joystick.c \
	glfw/win32/win32_thread.c \
	glfw/win32/win32_time.c \
	glfw/win32/win32_window.c

unix:!macx:HEADERS += glfw/x11/platform.h
macx:HEADERS += glfw/cocoa/platform.h
win32:HEADERS += glfw/win32/platform.h

HEADERS += yajl/api/yajl_version.h \
	yajl/api/yajl_tree.h \
	yajl/api/yajl_parse.h \
	yajl/api/yajl_gen.h \
	yajl/api/yajl_common.h \
	yajl/yajl_buf.h \
	yajl/yajl_alloc.h \
	yajl/yajl_parser.h \
	yajl/yajl_lex.h \
	yajl/yajl_encode.h \
	yajl/yajl_bytestack.h \
	vorbis/bitrate.h \
	vorbis/window.h \
	vorbis/vorbisfile.h \
	vorbis/vorbisenc.h \
	vorbis/smallft.h \
	vorbis/scales.h \
	vorbis/registry.h \
	vorbis/psy.h \
	vorbis/os.h \
	vorbis/misc.h \
	vorbis/mdct.h \
	vorbis/masking.h \
	vorbis/lsp.h \
	vorbis/lpc.h \
	vorbis/lookup_data.h \
	vorbis/lookup.h \
	vorbis/highlevel.h \
	vorbis/envelope.h \
	vorbis/codec_internal.h \
	vorbis/codec.h \
	vorbis/codebook.h \
	vorbis/backends.h \
	vorbis/modes/setup_X.h \
	vorbis/modes/setup_44u.h \
	vorbis/modes/setup_44p51.h \
	vorbis/modes/setup_44.h \
	vorbis/modes/setup_32.h \
	vorbis/modes/setup_22.h \
	vorbis/modes/setup_16.h \
	vorbis/modes/setup_11.h \
	vorbis/modes/setup_8.h \
	vorbis/modes/residue_44u.h \
	vorbis/modes/residue_44p51.h \
	vorbis/modes/residue_44.h \
	vorbis/modes/residue_16.h \
	vorbis/modes/residue_8.h \
	vorbis/modes/psych_44.h \
	vorbis/modes/psych_16.h \
	vorbis/modes/psych_11.h \
	vorbis/modes/psych_8.h \
	vorbis/modes/floor_all.h \
	vorbis/books/uncoupled/res_books_uncoupled.h \
	vorbis/books/floor/floor_books.h \
	vorbis/books/coupled/res_books_stereo.h \
	vorbis/books/coupled/res_books_51.h \
	theora/theoraenc.h \
	theora/theoradec.h \
	theora/theora.h \
	theora/quant.h \
	theora/ocintrin.h \
	theora/modedec.h \
	theora/mathops.h \
	theora/internal.h \
	theora/huffman.h \
	theora/huffenc.h \
	theora/huffdec.h \
	theora/enquant.h \
	theora/encint.h \
	theora/dequant.h \
	theora/decint.h \
	theora/dct.h \
	theora/cpu.h \
	theora/codec.h \
	theora/bitpack.h \
	theora/apiwrapper.h \
	physfs/physfs_platforms.h \
	physfs/physfs_internal.h \
	physfs/physfs_casefolding.h \
	physfs/physfs.h \
	physfs/zlib123/zutil.h \
	physfs/zlib123/trees.h \
	physfs/zlib123/zlib.h \
	physfs/zlib123/zconf.h \
	physfs/zlib123/inftrees.h \
	physfs/zlib123/inflate.h \
	physfs/zlib123/inffixed.h \
	physfs/zlib123/inffast.h \
	physfs/zlib123/deflate.h \
	physfs/zlib123/crc32.h \
	oggz/oggz_write.h \
	oggz/oggz_vector.h \
	oggz/oggz_table.h \
	oggz/oggz_stream_private.h \
	oggz/oggz_stream.h \
	oggz/oggz_seek.h \
	oggz/oggz_read.h \
	oggz/oggz_private.h \
	oggz/oggz_packet.h \
	oggz/oggz_off_t.h \
	oggz/oggz_macros.h \
	oggz/oggz_io.h \
	oggz/oggz_dlist.h \
	oggz/oggz_deprecated.h \
	oggz/oggz_constants.h \
	oggz/oggz_compat.h \
	oggz/oggz_comments.h \
	oggz/oggz_byteorder.h \
	oggz/oggz_auto.h \
	oggz/oggz.h \
	oggz/dirac.h \
	oggz/config.h \
	oggplay/std_semaphore.h \
	oggplay/oggplay_yuv2rgb_template.h \
	oggplay/oggplay_tools.h \
	oggplay/oggplay_tcp_reader.h \
	oggplay/oggplay_seek.h \
	oggplay/oggplay_reader.h \
	oggplay/oggplay_query.h \
	oggplay/oggplay_private.h \
	oggplay/oggplay_file_reader.h \
	oggplay/oggplay_enums.h \
	oggplay/oggplay_data.h \
	oggplay/oggplay_config_win32.h \
	oggplay/oggplay_config.h \
	oggplay/oggplay_callback_info.h \
	oggplay/oggplay_callback.h \
	oggplay/oggplay_buffer.h \
	oggplay/oggplay.h \
	oggplay/cpu.h \
	oggplay/x86/yuv2rgb_x86_vs.h \
	oggplay/x86/yuv2rgb_x86.h \
	ogg/os_types.h \
	ogg/ogg.h \
	glew/wglew.h \
	glew/glxew.h \
	glew/glew.h \
	Box2D/Box2D.h \
	Box2D/Collision/b2TimeOfImpact.h \
	Box2D/Collision/b2DynamicTree.h \
	Box2D/Collision/b2Distance.h \
	Box2D/Collision/b2Collision.h \
	Box2D/Collision/b2BroadPhase.h \
	Box2D/Collision/Shapes/b2Shape.h \
	Box2D/Collision/Shapes/b2ChainShape.h \
	Box2D/Collision/Shapes/b2EdgeShape.h \
	Box2D/Collision/Shapes/b2PolygonShape.h \
	Box2D/Collision/Shapes/b2CircleShape.h \
	Box2D/Common/b2Draw.h \
	Box2D/Common/b2GrowableStack.h \
	Box2D/Common/b2Settings.h \
	Box2D/Common/b2StackAllocator.h \
	Box2D/Common/b2Settings.h \
	Box2D/Common/b2Math.h \
	Box2D/Common/b2Timer.h \
	Box2D/Common/b2BlockAllocator.h \
	Box2D/Dynamics/b2WorldCallbacks.h \
	Box2D/Dynamics/b2World.h \
	Box2D/Dynamics/b2TimeStep.h \
	Box2D/Dynamics/b2Island.h \
	Box2D/Dynamics/b2Fixture.h \
	Box2D/Dynamics/b2ContactManager.h \
	Box2D/Dynamics/b2Body.h \
	Box2D/Dynamics/Contacts/b2ContactSolver.h \
	Box2D/Dynamics/Contacts/b2PolygonContact.h \
	Box2D/Dynamics/Contacts/b2Contact.h \
	Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
	Box2D/Dynamics/Contacts/b2CircleContact.h \
	Box2D/Dynamics/Joints/b2WeldJoint.h \
	Box2D/Dynamics/Joints/b2RevoluteJoint.h \
	Box2D/Dynamics/Joints/b2PulleyJoint.h \
	Box2D/Dynamics/Joints/b2PrismaticJoint.h \
	Box2D/Dynamics/Joints/b2MouseJoint.h \
	Box2D/Dynamics/Joints/b2Joint.h \
	Box2D/Dynamics/Joints/b2GearJoint.h \
	Box2D/Dynamics/Joints/b2FrictionJoint.h \
	Box2D/Dynamics/Joints/b2DistanceJoint.h \
	Box2D/Rope/b2Rope.h \
	vectormath/c/vectormath_soa_v.h \
	vectormath/c/vectormath_soa.h \
	vectormath/c/vectormath_aos_v.h \
	vectormath/c/vectormath_aos.h \
	vectormath/cpp/vectormath_soa.h \
	vectormath/cpp/vectormath_aos.h \
	vectormath/ios/vectormath_aos.h \
	vectormath/ios/vec_aos.h \
	vectormath/ios/quat_aos.h \
	vectormath/ios/mat_aos.h \
	vectormath/ios/floatInVec.h \
	vectormath/ios/boolInVec.h \
	vectormath/ppu/c/vectormath_soa_v.h \
	vectormath/ppu/c/vectormath_soa.h \
	vectormath/ppu/c/vectormath_aos_v.h \
	vectormath/ppu/c/vectormath_aos.h \
	vectormath/ppu/c/vec_types.h \
	vectormath/ppu/c/vec_soa_v.h \
	vectormath/ppu/c/vec_soa.h \
	vectormath/ppu/c/vec_aos_v.h \
	vectormath/ppu/c/quat_aos_v.h \
	vectormath/ppu/c/quat_aos.h \
	vectormath/ppu/c/mat_soa_v.h \
	vectormath/ppu/c/vec_aos.h \
	vectormath/ppu/c/quat_soa_v.h \
	vectormath/ppu/c/quat_soa.h \
	vectormath/ppu/c/mat_soa.h \
	vectormath/ppu/c/mat_aos_v.h \
	vectormath/ppu/c/mat_aos.h \
	vectormath/ppu/cpp/vectormath_soa.h \
	vectormath/ppu/cpp/vectormath_aos.h \
	vectormath/ppu/cpp/quat_soa.h \
	vectormath/ppu/cpp/quat_aos.h \
	vectormath/ppu/cpp/mat_soa.h \
	vectormath/ppu/cpp/vecidx_aos.h \
	vectormath/ppu/cpp/vec_soa.h \
	vectormath/ppu/cpp/vec_aos.h \
	vectormath/ppu/cpp/mat_aos.h \
	vectormath/ppu/cpp/floatInVec.h \
	vectormath/ppu/cpp/boolInVec.h \
	vectormath/scalar/c/vec_aos_v.h \
	vectormath/scalar/c/vec_aos.h \
	vectormath/scalar/c/quat_aos_v.h \
	vectormath/scalar/c/vectormath_aos_v.h \
	vectormath/scalar/c/vectormath_aos.h \
	vectormath/scalar/c/quat_aos.h \
	vectormath/scalar/c/mat_aos_v.h \
	vectormath/scalar/c/mat_aos.h \
	vectormath/scalar/cpp/vectormath_aos.h \
	vectormath/scalar/cpp/vec_aos.h \
	vectormath/scalar/cpp/quat_aos.h \
	vectormath/scalar/cpp/mat_aos.h \
	vectormath/scalar/cpp/floatInVec.h \
	vectormath/scalar/cpp/boolInVec.h \
	vectormath/spu/c/vectormath_soa_v.h \
	vectormath/spu/c/vectormath_soa.h \
	vectormath/spu/c/vectormath_aos_v.h \
	vectormath/spu/c/vectormath_aos.h \
	vectormath/spu/c/vec_soa_v.h \
	vectormath/spu/c/vec_soa.h \
	vectormath/spu/c/vec_aos_v.h \
	vectormath/spu/c/quat_aos_v.h \
	vectormath/spu/c/quat_aos.h \
	vectormath/spu/c/mat_soa_v.h \
	vectormath/spu/c/vec_aos.h \
	vectormath/spu/c/quat_soa_v.h \
	vectormath/spu/c/quat_soa.h \
	vectormath/spu/c/mat_soa.h \
	vectormath/spu/c/mat_aos_v.h \
	vectormath/spu/c/mat_aos.h \
	vectormath/spu/cpp/vectormath_soa.h \
	vectormath/spu/cpp/vectormath_aos.h \
	vectormath/spu/cpp/quat_soa.h \
	vectormath/spu/cpp/quat_aos.h \
	vectormath/spu/cpp/mat_soa.h \
	vectormath/spu/cpp/vecidx_aos.h \
	vectormath/spu/cpp/vec_soa.h \
	vectormath/spu/cpp/vec_aos.h \
	vectormath/spu/cpp/mat_aos.h \
	vectormath/spu/cpp/floatInVec.h \
	vectormath/spu/cpp/boolInVec.h \
	vectormath/SSE/cpp/vec_aos.h \
	vectormath/SSE/cpp/quat_aos.h \
	vectormath/SSE/cpp/vectormath_aos.h \
	vectormath/SSE/cpp/vecidx_aos.h \
	vectormath/SSE/cpp/mat_aos.h \
	vectormath/SSE/cpp/floatInVec.h \
	vectormath/SSE/cpp/boolInVec.h \
	vectormath/vfpmath/vsinf.h \
	vectormath/vfpmath/vfp_clobbers.h \
	vectormath/vfpmath/matrix_impl.h \
	vectormath/vfpmath/common_macros.h \
	lua/lzio.h \
	lua/lvm.h \
	lua/lundump.h \
	lua/lualib.h \
	lua/luaconf.h \
	lua/lua.hpp \
	lua/lua.h \
	lua/ltm.h \
	lua/ltable.h \
	lua/lstring.h \
	lua/lstate.h \
	lua/lparser.h \
	lua/lopcodes.h \
	lua/lobject.h \
	lua/lmem.h \
	lua/llimits.h \
	lua/llex.h \
	lua/lgc.h \
	lua/lfunc.h \
	lua/ldo.h \
	lua/ldebug.h \
	lua/lctype.h \
	lua/lcode.h \
	lua/lauxlib.h \
	lua/lapi.h \
	glfw/internal.h \
	glfw/glfw.h \
	soil/stbi_DDS_aug.h \
	soil/stbi_DDS_aug_c.h \
	soil/stb_image_aug.h \
	soil/SOIL.h \
	soil/image_helper.h \
	soil/image_DXT.h \
	freetype/config/ftconfig.h \
	freetype/config/ftheader.h \
	freetype/config/ftmodule.h \
	freetype/config/ftoption.h \
	freetype/config/ftstdlib.h \
	freetype/freetype.h \
	freetype/ft2build.h \
	freetype/ftadvanc.h \
	freetype/ftbbox.h \
	freetype/ftbdf.h \
	freetype/ftbitmap.h \
	freetype/ftbzip2.h \
	freetype/ftcache.h \
	freetype/ftchapters.h \
	freetype/ftcid.h \
	freetype/fterrdef.h \
	freetype/fterrors.h \
	freetype/ftgasp.h \
	freetype/ftglyph.h \
	freetype/ftgxval.h \
	freetype/ftgzip.h \
	freetype/ftimage.h \
	freetype/ftincrem.h \
	freetype/ftlcdfil.h \
	freetype/ftlist.h \
	freetype/ftlzw.h \
	freetype/ftmac.h \
	freetype/ftmm.h \
	freetype/ftmodapi.h \
	freetype/ftmoderr.h \
	freetype/ftoption.h \
	freetype/ftotval.h \
	freetype/ftoutln.h \
	freetype/ftpfr.h \
	freetype/ftrender.h \
	freetype/ftsizes.h \
	freetype/ftsnames.h \
	freetype/ftstroke.h \
	freetype/ftsynth.h \
	freetype/ftsystem.h \
	freetype/fttrigon.h \
	freetype/fttypes.h \
	freetype/ftwinfnt.h \
	freetype/ftxf86.h \
	freetype/internal/autohint.h \
	freetype/internal/ftcalc.h \
	freetype/internal/ftdebug.h \
	freetype/internal/ftdriver.h \
	freetype/internal/ftgloadr.h \
	freetype/internal/ftmemory.h \
	freetype/internal/ftobjs.h \
	freetype/internal/ftpic.h \
	freetype/internal/ftrfork.h \
	freetype/internal/ftserv.h \
	freetype/internal/ftstream.h \
	freetype/internal/fttrace.h \
	freetype/internal/ftvalid.h \
	freetype/internal/internal.h \
	freetype/internal/psaux.h \
	freetype/internal/pshints.h \
	freetype/internal/services/svbdf.h \
	freetype/internal/services/svcid.h \
	freetype/internal/services/svgldict.h \
	freetype/internal/services/svgxval.h \
	freetype/internal/services/svkern.h \
	freetype/internal/services/svmm.h \
	freetype/internal/services/svotval.h \
	freetype/internal/services/svpfr.h \
	freetype/internal/services/svpostnm.h \
	freetype/internal/services/svpscmap.h \
	freetype/internal/services/svpsinfo.h \
	freetype/internal/services/svsfnt.h \
	freetype/internal/services/svttcmap.h \
	freetype/internal/services/svtteng.h \
	freetype/internal/services/svttglyf.h \
	freetype/internal/services/svwinfnt.h \
	freetype/internal/services/svxf86nm.h \
	freetype/internal/sfnt.h \
	freetype/internal/t1types.h \
	freetype/internal/tttypes.h \
	freetype/src/autofit/afangles.h \
	freetype/src/autofit/afcjk.h \
	freetype/src/autofit/afdummy.h \
	freetype/src/autofit/aferrors.h \
	freetype/src/autofit/afglobal.h \
	freetype/src/autofit/afhints.h \
	freetype/src/autofit/afindic.h \
	freetype/src/autofit/aflatin.h \
	freetype/src/autofit/aflatin2.h \
	freetype/src/autofit/afloader.h \
	freetype/src/autofit/afmodule.h \
	freetype/src/autofit/afpic.h \
	freetype/src/autofit/aftypes.h \
	freetype/src/autofit/afwarp.h \
	freetype/src/base/basepic.h \
	freetype/src/base/ftbase.h \
	freetype/src/bdf/bdf.h \
	freetype/src/bdf/bdfdrivr.h \
	freetype/src/bdf/bdferror.h \
	freetype/src/cache/ftccache.h \
	freetype/src/cache/ftccback.h \
	freetype/src/cache/ftcerror.h \
	freetype/src/cache/ftcglyph.h \
	freetype/src/cache/ftcimage.h \
	freetype/src/cache/ftcmanag.h \
	freetype/src/cache/ftcmru.h \
	freetype/src/cache/ftcsbits.h \
	freetype/src/cff/cffcmap.h \
	freetype/src/cff/cffdrivr.h \
	freetype/src/cff/cfferrs.h \
	freetype/src/cff/cffgload.h \
	freetype/src/cff/cffload.h \
	freetype/src/cff/cffobjs.h \
	freetype/src/cff/cffparse.h \
	freetype/src/cff/cffpic.h \
	freetype/src/cff/cfftoken.h \
	freetype/src/cff/cfftypes.h \
	freetype/src/cid/ciderrs.h \
	freetype/src/cid/cidgload.h \
	freetype/src/cid/cidload.h \
	freetype/src/cid/cidobjs.h \
	freetype/src/cid/cidparse.h \
	freetype/src/cid/cidriver.h \
	freetype/src/cid/cidtoken.h \
	freetype/src/gxvalid/gxvalid.h \
	freetype/src/gxvalid/gxvcommn.h \
	freetype/src/gxvalid/gxverror.h \
	freetype/src/gxvalid/gxvfeat.h \
	freetype/src/gxvalid/gxvmod.h \
	freetype/src/gxvalid/gxvmort.h \
	freetype/src/gxvalid/gxvmorx.h \
	freetype/src/gzip/infblock.h \
	freetype/src/gzip/infcodes.h \
	freetype/src/gzip/inffixed.h \
	freetype/src/gzip/inftrees.h \
	freetype/src/gzip/infutil.h \
	freetype/src/gzip/zconf.h \
	freetype/src/gzip/zlib.h \
	freetype/src/gzip/zutil.h \
	freetype/src/lzw/ftzopen.h \
	freetype/src/otvalid/otvalid.h \
	freetype/src/otvalid/otvcommn.h \
	freetype/src/otvalid/otverror.h \
	freetype/src/otvalid/otvgpos.h \
	freetype/src/otvalid/otvmod.h \
	freetype/src/pcf/pcf.h \
	freetype/src/pcf/pcfdrivr.h \
	freetype/src/pcf/pcferror.h \
	freetype/src/pcf/pcfread.h \
	freetype/src/pcf/pcfutil.h \
	freetype/src/pfr/pfrcmap.h \
	freetype/src/pfr/pfrdrivr.h \
	freetype/src/pfr/pfrerror.h \
	freetype/src/pfr/pfrgload.h \
	freetype/src/pfr/pfrload.h \
	freetype/src/pfr/pfrobjs.h \
	freetype/src/pfr/pfrsbit.h \
	freetype/src/pfr/pfrtypes.h \
	freetype/src/psaux/afmparse.h \
	freetype/src/psaux/psauxerr.h \
	freetype/src/psaux/psauxmod.h \
	freetype/src/psaux/psconv.h \
	freetype/src/psaux/psobjs.h \
	freetype/src/psaux/t1cmap.h \
	freetype/src/psaux/t1decode.h \
	freetype/src/pshinter/pshalgo.h \
	freetype/src/pshinter/pshglob.h \
	freetype/src/pshinter/pshmod.h \
	freetype/src/pshinter/pshnterr.h \
	freetype/src/pshinter/pshpic.h \
	freetype/src/pshinter/pshrec.h \
	freetype/src/psnames/psmodule.h \
	freetype/src/psnames/psnamerr.h \
	freetype/src/psnames/pspic.h \
	freetype/src/psnames/pstables.h \
	freetype/src/raster/ftmisc.h \
	freetype/src/raster/ftraster.h \
	freetype/src/raster/ftrend1.h \
	freetype/src/raster/rasterrs.h \
	freetype/src/raster/rastpic.h \
	freetype/src/sfnt/sfdriver.h \
	freetype/src/sfnt/sferrors.h \
	freetype/src/sfnt/sfntpic.h \
	freetype/src/sfnt/sfobjs.h \
	freetype/src/sfnt/ttbdf.h \
	freetype/src/sfnt/ttcmap.h \
	freetype/src/sfnt/ttcmapc.h \
	freetype/src/sfnt/ttkern.h \
	freetype/src/sfnt/ttload.h \
	freetype/src/sfnt/ttmtx.h \
	freetype/src/sfnt/ttpost.h \
	freetype/src/sfnt/ttsbit.h \
	freetype/src/smooth/ftgrays.h \
	freetype/src/smooth/ftsmerrs.h \
	freetype/src/smooth/ftsmooth.h \
	freetype/src/smooth/ftspic.h \
	freetype/src/truetype/ttdriver.h \
	freetype/src/truetype/tterrors.h \
	freetype/src/truetype/ttgload.h \
	freetype/src/truetype/ttgxvar.h \
	freetype/src/truetype/ttinterp.h \
	freetype/src/truetype/ttobjs.h \
	freetype/src/truetype/ttpic.h \
	freetype/src/truetype/ttpload.h \
	freetype/src/type1/t1afm.h \
	freetype/src/type1/t1driver.h \
	freetype/src/type1/t1errors.h \
	freetype/src/type1/t1gload.h \
	freetype/src/type1/t1load.h \
	freetype/src/type1/t1objs.h \
	freetype/src/type1/t1parse.h \
	freetype/src/type1/t1tokens.h \
	freetype/src/type42/t42drivr.h \
	freetype/src/type42/t42error.h \
	freetype/src/type42/t42objs.h \
	freetype/src/type42/t42parse.h \
	freetype/src/type42/t42types.h \
	freetype/src/winfonts/fnterrs.h \
	freetype/src/winfonts/winfnt.h \
	freetype/t1tables.h \
	freetype/ttnameid.h \
	freetype/tttables.h \
	freetype/tttags.h \
	freetype/ttunpat.h \
	Rocket/Controls/Clipboard.h \
	Rocket/Controls/Controls.h \
	Rocket/Controls/DataFormatter.h \
	Rocket/Controls/DataQuery.h \
	Rocket/Controls/DataSource.h \
	Rocket/Controls/DataSourceListener.h \
	Rocket/Controls/ElementDataGrid.h \
	Rocket/Controls/ElementDataGridCell.h \
	Rocket/Controls/ElementDataGridExpandButton.h \
	Rocket/Controls/ElementDataGridRow.h \
	Rocket/Controls/ElementForm.h \
	Rocket/Controls/ElementFormControl.h \
	Rocket/Controls/ElementFormControlDataSelect.h \
	Rocket/Controls/ElementFormControlInput.h \
	Rocket/Controls/ElementFormControlSelect.h \
	Rocket/Controls/ElementFormControlTextArea.h \
	Rocket/Controls/ElementTabSet.h \
	Rocket/Controls/Header.h \
	Rocket/Controls/SelectOption.h \
	Rocket/Controls.h \
	Rocket/Core/BaseXMLParser.h \
	Rocket/Core/Box.h \
	Rocket/Core/Colour.h \
	Rocket/Core/Context.h \
	Rocket/Core/ContextInstancer.h \
	Rocket/Core/ConvolutionFilter.h \
	Rocket/Core/Core.h \
	Rocket/Core/Debug.h \
	Rocket/Core/Decorator.h \
	Rocket/Core/DecoratorInstancer.h \
	Rocket/Core/Dictionary.h \
	Rocket/Core/Element.h \
	Rocket/Core/ElementDocument.h \
	Rocket/Core/ElementInstancer.h \
	Rocket/Core/ElementInstancerGeneric.h \
	Rocket/Core/ElementReference.h \
	Rocket/Core/ElementScroll.h \
	Rocket/Core/ElementText.h \
	Rocket/Core/ElementUtilities.h \
	Rocket/Core/Event.h \
	Rocket/Core/EventInstancer.h \
	Rocket/Core/EventListener.h \
	Rocket/Core/EventListenerInstancer.h \
	Rocket/Core/Factory.h \
	Rocket/Core/FileInterface.h \
	Rocket/Core/Font.h \
	Rocket/Core/FontDatabase.h \
	Rocket/Core/FontEffect.h \
	Rocket/Core/FontEffectInstancer.h \
	Rocket/Core/FontGlyph.h \
	Rocket/Core/Geometry.h \
	Rocket/Core/GeometryUtilities.h \
	Rocket/Core/Header.h \
	Rocket/Core/Input.h \
	Rocket/Core/Log.h \
	Rocket/Core/Math.h \
	Rocket/Core/MathTypes.h \
	Rocket/Core/Platform.h \
	Rocket/Core/Plugin.h \
	Rocket/Core/Property.h \
	Rocket/Core/PropertyDefinition.h \
	Rocket/Core/PropertyDictionary.h \
	Rocket/Core/PropertyParser.h \
	Rocket/Core/PropertySpecification.h \
	Rocket/Core/ReferenceCountable.h \
	Rocket/Core/RenderInterface.h \
	Rocket/Core/ScriptInterface.h \
	Rocket/Core/Stream.h \
	Rocket/Core/StreamMemory.h \
	Rocket/Core/String.h \
	Rocket/Core/StringBase.h \
	Rocket/Core/StringUtilities.h \
	Rocket/Core/StyleSheet.h \
	Rocket/Core/StyleSheetKeywords.h \
	Rocket/Core/StyleSheetSpecification.h \
	Rocket/Core/SystemInterface.h \
	Rocket/Core/Texture.h \
	Rocket/Core/TypeConverter.h \
	Rocket/Core/Types.h \
	Rocket/Core/URL.h \
	Rocket/Core/Variant.h \
	Rocket/Core/Vector2.h \
	Rocket/Core/Vertex.h \
	Rocket/Core/WString.h \
	Rocket/Core/XMLNodeHandler.h \
	Rocket/Core/XMLParser.h \
	Rocket/Core.h \
	Rocket/Debugger/Debugger.h \
	Rocket/Debugger/Header.h \
	Rocket/Debugger.h \
	Rocket/Source/Controls/ElementTextSelection.h \
	Rocket/Source/Controls/InputType.h \
	Rocket/Source/Controls/InputTypeButton.h \
	Rocket/Source/Controls/InputTypeCheckbox.h \
	Rocket/Source/Controls/InputTypeRadio.h \
	Rocket/Source/Controls/InputTypeRange.h \
	Rocket/Source/Controls/InputTypeSubmit.h \
	Rocket/Source/Controls/InputTypeText.h \
	Rocket/Source/Controls/WidgetDropDown.h \
	Rocket/Source/Controls/WidgetSlider.h \
	Rocket/Source/Controls/WidgetSliderInput.h \
	Rocket/Source/Controls/WidgetTextInput.h \
	Rocket/Source/Controls/WidgetTextInputMultiLine.h \
	Rocket/Source/Controls/WidgetTextInputSingleLine.h \
	Rocket/Source/Controls/WidgetTextInputSingleLinePassword.h \
	Rocket/Source/Controls/XMLNodeHandlerDataGrid.h \
	Rocket/Source/Controls/XMLNodeHandlerTabSet.h \
	Rocket/Source/Controls/XMLNodeHandlerTextArea.h \
	Rocket/Source/Core/Clock.h \
	Rocket/Source/Core/ContextInstancerDefault.h \
	Rocket/Source/Core/DebugFont.h \
	Rocket/Source/Core/DecoratorNone.h \
	Rocket/Source/Core/DecoratorNoneInstancer.h \
	Rocket/Source/Core/DecoratorTiled.h \
	Rocket/Source/Core/DecoratorTiledBox.h \
	Rocket/Source/Core/DecoratorTiledBoxInstancer.h \
	Rocket/Source/Core/DecoratorTiledHorizontal.h \
	Rocket/Source/Core/DecoratorTiledHorizontalInstancer.h \
	Rocket/Source/Core/DecoratorTiledImage.h \
	Rocket/Source/Core/DecoratorTiledImageInstancer.h \
	Rocket/Source/Core/DecoratorTiledInstancer.h \
	Rocket/Source/Core/DecoratorTiledVertical.h \
	Rocket/Source/Core/DecoratorTiledVerticalInstancer.h \
	Rocket/Source/Core/DocumentHeader.h \
	Rocket/Source/Core/ElementBackground.h \
	Rocket/Source/Core/ElementBorder.h \
	Rocket/Source/Core/ElementDecoration.h \
	Rocket/Source/Core/ElementDefinition.h \
	Rocket/Source/Core/ElementHandle.h \
	Rocket/Source/Core/ElementImage.h \
	Rocket/Source/Core/ElementStyle.h \
	Rocket/Source/Core/ElementTextDefault.h \
	Rocket/Source/Core/EventDispatcher.h \
	Rocket/Source/Core/EventInstancerDefault.h \
	Rocket/Source/Core/EventIterators.h \
	Rocket/Source/Core/FileInterfaceDefault.h \
	Rocket/Source/Core/FontEffectNone.h \
	Rocket/Source/Core/FontEffectNoneInstancer.h \
	Rocket/Source/Core/FontEffectOutline.h \
	Rocket/Source/Core/FontEffectOutlineInstancer.h \
	Rocket/Source/Core/FontEffectShadow.h \
	Rocket/Source/Core/FontEffectShadowInstancer.h \
	Rocket/Source/Core/FontFace.h \
	Rocket/Source/Core/FontFaceHandle.h \
	Rocket/Source/Core/FontFaceLayer.h \
	Rocket/Source/Core/FontFamily.h \
	Rocket/Source/Core/GeometryDatabase.h \
	Rocket/Source/Core/LayoutBlockBox.h \
	Rocket/Source/Core/LayoutBlockBoxSpace.h \
	Rocket/Source/Core/LayoutEngine.h \
	Rocket/Source/Core/LayoutInlineBox.h \
	Rocket/Source/Core/LayoutInlineBoxText.h \
	Rocket/Source/Core/LayoutLineBox.h \
	Rocket/Source/Core/PluginRegistry.h \
	Rocket/Source/Core/Pool.h \
	Rocket/Source/Core/precompiled.h \
	Rocket/Source/Core/PropertyParserColour.h \
	Rocket/Source/Core/PropertyParserKeyword.h \
	Rocket/Source/Core/PropertyParserNumber.h \
	Rocket/Source/Core/PropertyParserString.h \
	Rocket/Source/Core/PropertyShorthandDefinition.h \
	Rocket/Source/Core/StreamFile.h \
	Rocket/Source/Core/StringCache.h \
	Rocket/Source/Core/StyleSheetFactory.h \
	Rocket/Source/Core/StyleSheetNode.h \
	Rocket/Source/Core/StyleSheetNodeSelector.h \
	Rocket/Source/Core/StyleSheetNodeSelectorEmpty.h \
	Rocket/Source/Core/StyleSheetNodeSelectorFirstChild.h \
	Rocket/Source/Core/StyleSheetNodeSelectorFirstOfType.h \
	Rocket/Source/Core/StyleSheetNodeSelectorLastChild.h \
	Rocket/Source/Core/StyleSheetNodeSelectorLastOfType.h \
	Rocket/Source/Core/StyleSheetNodeSelectorNthChild.h \
	Rocket/Source/Core/StyleSheetNodeSelectorNthLastChild.h \
	Rocket/Source/Core/StyleSheetNodeSelectorNthLastOfType.h \
	Rocket/Source/Core/StyleSheetNodeSelectorNthOfType.h \
	Rocket/Source/Core/StyleSheetNodeSelectorOnlyChild.h \
	Rocket/Source/Core/StyleSheetNodeSelectorOnlyOfType.h \
	Rocket/Source/Core/StyleSheetParser.h \
	Rocket/Source/Core/Template.h \
	Rocket/Source/Core/TemplateCache.h \
	Rocket/Source/Core/TextureDatabase.h \
	Rocket/Source/Core/TextureLayout.h \
	Rocket/Source/Core/TextureLayoutRectangle.h \
	Rocket/Source/Core/TextureLayoutRow.h \
	Rocket/Source/Core/TextureLayoutTexture.h \
	Rocket/Source/Core/TextureResource.h \
	Rocket/Source/Core/UnicodeRange.h \
	Rocket/Source/Core/WidgetSlider.h \
	Rocket/Source/Core/WidgetSliderScroll.h \
	Rocket/Source/Core/XMLNodeHandlerBody.h \
	Rocket/Source/Core/XMLNodeHandlerDefault.h \
	Rocket/Source/Core/XMLNodeHandlerHead.h \
	Rocket/Source/Core/XMLNodeHandlerTemplate.h \
	Rocket/Source/Core/XMLParseTools.h \
	Rocket/Source/Debugger/BeaconSource.h \
	Rocket/Source/Debugger/CommonSource.h \
	Rocket/Source/Debugger/ElementContextHook.h \
	Rocket/Source/Debugger/ElementInfo.h \
	Rocket/Source/Debugger/ElementLog.h \
	Rocket/Source/Debugger/FontSource.h \
	Rocket/Source/Debugger/Geometry.h \
	Rocket/Source/Debugger/InfoSource.h \
	Rocket/Source/Debugger/LogSource.h \
	Rocket/Source/Debugger/MenuSource.h \
	Rocket/Source/Debugger/Plugin.h \
	Rocket/Source/Debugger/SystemInterface.h \
	Rocket/Core/Colour.inl \
	Rocket/Core/Dictionary.inl \
	Rocket/Core/Element.inl \
	Rocket/Core/ElementInstancerGeneric.inl \
	Rocket/Core/Pool.inl \
	Rocket/Core/StringBase.inl \
	Rocket/Core/TypeConverter.inl \
	Rocket/Core/Variant.inl \
	Rocket/Core/Vector2.inl \
	Rocket/Source/Core/Pool.inl \
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


OTHER_FILES += \
	physfs/CREDITS.txt \
	physfs/LICENSE.txt \
	physfs/zlib123/README \
	physfs/extras/makecasefoldhashtable.pl \
	physfs/extras/casefolding.txt \
	Box2D/License.txt \
	vectormath/SSE/cpp/readme_e.txt \
	lua/Makefile \
	README.md \
	flash/Console.as

