ARCHIVE_DIR = client/target
PACKAGE_DIR = $(ARCHIVE_DIR)/package
ARCHIVE_NAME = cg_package.tar.gz
CONFIGURATION = Release
TARGZ_CREATE = tar -zcf

MAKE = make

all: build_engine build_client package

package: build_client
	mkdir -p $(PACKAGE_DIR)/
	cp engine/bin/$(CONFIGURATION)/libengine.so $(PACKAGE_DIR)/
	cp client/bin/$(CONFIGURATION)/client $(PACKAGE_DIR)/
	cp client/bin/$(CONFIGURATION)/*.ovo $(PACKAGE_DIR)/
	cp -r client/bin/$(CONFIGURATION)/textures/ $(PACKAGE_DIR)/
	$(TARGZ_CREATE) $(ARCHIVE_DIR)/$(ARCHIVE_NAME) -C $(PACKAGE_DIR)/ .
	rm -rf $(PACKAGE_DIR)

build_engine: 
	$(MAKE) -C engine all

build_client: build_engine
	$(MAKE) -C client all

clean: clean_engine clean_client

clean_engine: 
	$(MAKE) -C engine clean

clean_client: 
	$(MAKE) -C client clean

.PHONY: clean_engine clean_client package

