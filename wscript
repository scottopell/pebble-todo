
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

import os.path

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    ctx.load('pebble_sdk')

    build_worker = os.path.exists('worker_src')
    binaries = []

    for p in ctx.env.TARGET_PLATFORMS:
        ctx.set_env(ctx.all_envs[p])
        app_elf='{}/pebble-app.elf'.format(ctx.env.BUILD_DIR)
        ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'),
        target=app_elf)

        if build_worker:
            worker_elf='{}/pebble-worker.elf'.format(ctx.env.BUILD_DIR)
            binaries.append({'platform': p, 'app_elf': app_elf, 'worker_elf': worker_elf})
            ctx.pbl_worker(source=ctx.path.ant_glob('worker_src/**/*.c'),
            target=worker_elf)
        else:
            binaries.append({'platform': p, 'app_elf': app_elf})

    # compile any typescript files laying around
    ctx.exec_command('find . -type f -name "*.ts" -exec tsc {} \;')
    # Join all JS files (first subdirs, then root, then main.js):
    # get all js files..
    src_js = ctx.path.ant_glob('src/js/**/*.js')
    # get main.js node
    src_js_main = ctx.path.make_node('src/js/main.js')
    # get destination path for joined file (it will be in build/src/js/)
    build_js = ctx.path.get_bld().make_node('src/js/pebble-js-app.js')
    # check minify (uglifyjs)
    ctx(rule='(echo ${SRC}; env | grep "P_DEV" && cat ${SRC} > ${TGT} || uglifyjs ${SRC} -o ${TGT} )', source=src_js, target=build_js)

    ctx.pbl_bundle(binaries=binaries, js=ctx.path.ant_glob('src/js/**/*.js'))
