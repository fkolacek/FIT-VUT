# -*- encoding: utf-8 -*-
# stub: rkelly 1.0.7 ruby lib

Gem::Specification.new do |s|
  s.name = "rkelly"
  s.version = "1.0.7"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Aaron Patterson"]
  s.date = "2012-10-10"
  s.description = "The RKelly library will parse JavaScript and return a parse tree."
  s.email = ["aaron.patterson@gmail.com"]
  s.extra_rdoc_files = ["CHANGELOG.rdoc", "Manifest.txt", "README.rdoc"]
  s.files = ["CHANGELOG.rdoc", "Manifest.txt", "README.rdoc"]
  s.homepage = "http://rkelly.rubyforge.org/"
  s.rdoc_options = ["--main", "README.rdoc"]
  s.rubyforge_project = "rkelly"
  s.rubygems_version = "2.2.2"
  s.summary = "The RKelly library will parse JavaScript and return a parse tree."

  s.installed_by_version = "2.2.2" if s.respond_to? :installed_by_version

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<rdoc>, ["~> 3.10"])
      s.add_development_dependency(%q<hoe>, ["~> 3.0"])
    else
      s.add_dependency(%q<rdoc>, ["~> 3.10"])
      s.add_dependency(%q<hoe>, ["~> 3.0"])
    end
  else
    s.add_dependency(%q<rdoc>, ["~> 3.10"])
    s.add_dependency(%q<hoe>, ["~> 3.0"])
  end
end
