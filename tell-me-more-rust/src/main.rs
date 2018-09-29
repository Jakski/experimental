extern crate clap;

use clap::{Arg, App};
use std::env;
use std::fs::File;
use std::io::prelude::*;

const USERNAME_VAR: &'static str = "LOGNAME";
const PASSWD_FILE: &'static str = "/etc/passwd";
const UPTIME_FILE: &'static str = "/proc/uptime";
const MEMINFO_FILE: &'static str = "/proc/meminfo";

fn get_file(f: &str) -> (String) {
    let mut ret = String::new();
    File::open(f)
        .expect(&format!("Failed to open {}", f))
        .read_to_string(&mut ret)
        .expect(&format!("Failed to read {}", f));
    ret
}

fn main() {
    let matches = App::new("Tell-me-more")
        .author("Jakub Pieńkowski <jakub@nomail>")
        .about("Displays information about system")
        .arg(Arg::with_name("uptime")
             .short("u")
             .long("uptime")
             .help("Displays system uptime"))
        .arg(Arg::with_name("memory")
             .short("m")
             .long("memory")
             .help("Displays system memory"))
        .arg(Arg::with_name("whoami")
             .short("w")
             .long("whoami")
             .help("Displays current user"))
        .arg(Arg::with_name("users")
             .short("l")
             .long("users")
             .help("Displays system users"))
        .arg(Arg::with_name("no-bash")
             .short("n")
             .long("no-bash")
             .help("Displays system users without Bash"))
        .get_matches();
    if matches.is_present("uptime") {
        if let Some(line) = get_file(UPTIME_FILE).lines().next() {
            let minutes = (line.split(' ').next().unwrap()
                           .parse::<f32>().unwrap() / 60.0).round();
            println!("{} minutes", minutes);
        } else {
            eprintln!("No uptime information found!");
        }
    } else if matches.is_present("memory") {
        for line in get_file(MEMINFO_FILE).lines() {
            let line: Vec<&str> = line.split(' ').filter(|i| !i.is_empty()).collect();
            if let Some(property) = line.get(0) {
                if property.starts_with("Mem") {
                    let value = line[1].parse::<i32>().unwrap();
                    println!("{} {}", property, value / 1024);
                }
            }
        }
    } else if matches.is_present("whoami") {
        match env::var(USERNAME_VAR) {
            Ok(user) => println!("{}", user),
            _ => println!("Failed to find variable {}", USERNAME_VAR)
        }
    } else if matches.is_present("users") {
        for line in get_file(PASSWD_FILE).lines() {
            if let Some(user) = line.split(':').next() {
                println!("{}", user);
            }
        }
    } else if matches.is_present("no-bash") {
        for line in get_file(PASSWD_FILE).lines() {
            let line: Vec<&str> = line.split(':').collect();
            if let Some(shell) = line.get(6) {
                if !shell.ends_with("bash") {
                    println!("{}", line[0]);
                }
            }
        }
    }
}
